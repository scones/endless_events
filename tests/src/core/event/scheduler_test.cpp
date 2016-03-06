/*
 * scheduler_test.cpp
 *
 *  Created on: 05.03.2016
 *      Author: scones
 */


#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>


#include "core/event/scheduler.h"


#include <chrono>
#include <thread>


std::uint64_t callback_call_counter(0);


void callback_test_function(core::event::event const& e) {
  BOOST_CHECK("callback_test_event_2" == e.get_type());
  ++callback_call_counter;
}


BOOST_AUTO_TEST_SUITE(scheduler_test)


BOOST_AUTO_TEST_CASE(enqueue_swap_test) {
  core::event::scheduler x;
  x.enqueue({"scheduler test event", {{"id", std::uint64_t(33)}, {"some_var", "some value"}}});

  core::event::scheduler::t_event_vector* events;
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "there is a scheduled event in the front queue before the swap");

  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(1 == events->size(), "scheduled event missing after swap");
  BOOST_CHECK_MESSAGE(33 == (*events)[0]->get_argument("id").get_uint_value(), "wrong event pulled from scheduler");

  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "scheduled event should be swapped out");

  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "scheduled event should not reappear");
}


BOOST_AUTO_TEST_CASE(enqueue_frame_delayed_swap_test) {
  core::event::scheduler x;
  core::event::scheduler::t_event_vector* events;

  x.enqueue_frame_delayed({"some other event", {{"id", std::uint64_t(2)}}}, 2);

  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "there is a scheduled event in the front queue before the swap");

  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "scheduled event should not appear on the first frame");

  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(1 == events->size(), "scheduled event should appear on the second frame");
  BOOST_CHECK_MESSAGE(2 == (*events)[0]->get_argument("id").get_uint_value(), "wrong event pulled from scheduler");

  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "scheduled event should be swapped out");

  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "scheduled event should not reappear");
}


BOOST_AUTO_TEST_CASE(enqueue_time_delayed_swap_test) {
  core::event::scheduler x;
  core::event::scheduler::t_event_vector const* events;

  x.enqueue_time_delayed({"some other event", {{"id", std::uint64_t(3)}}}, 530);

  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "there is a scheduled event in the front queue before the swap");

  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "scheduled event should not appear at first");

  std::this_thread::sleep_for(std::chrono::milliseconds(510));
  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "scheduled event should not shortly before the time has arrived (test is unstable of course)");

  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(1 == events->size(), "scheduled event should appear when the time has arrived");
  BOOST_CHECK_MESSAGE(3 == (*events)[0]->get_argument("id").get_uint_value(), "wrong event pulled from scheduler");

  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "scheduled event should be swapped out");

  x.swap();
  events = x.poll_events();
  BOOST_CHECK_MESSAGE(0 == events->size(), "scheduled event should not reappear");
}


BOOST_AUTO_TEST_CASE(poll_events_test) {
  core::event::scheduler x;

  x.enqueue({"callback_test_event_1", {{"id", std::uint64_t(5)}, {"some_var", "some_value"}}});
  x.swap();
  BOOST_CHECK(0 == callback_call_counter);

  x.enqueue({"callback_test_event_2", {{"id", std::uint64_t(5)}, {"some_var", "some_value"}}});
  x.set_event_callback(callback_test_function);
  x.swap();
  x.poll_events();
  BOOST_CHECK(1 == callback_call_counter);
}


BOOST_AUTO_TEST_SUITE_END()

