/*
 * event_handler_test.cpp
 *
 *  Created on: 31.10.2015
 *      Author: scones
 */


#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>


#include "core/event/event_handler.h"


class event_handler_test_class : public core::event::event_receiver {

  public:

  event_handler_test_class() : core::event::event_receiver() {}
  ~event_handler_test_class() {};

};


BOOST_AUTO_TEST_SUITE(event_handler_test)


BOOST_AUTO_TEST_CASE(bind) {
  event_handler_test_class t1;
  event_handler_test_class t2;
  core::event::event_handler x;
  x.bind("32", &t1);
  x.bind("32", &t2);

  // fire event type 32
  x.propagate_event(core::event::event("32", {{"id", std::uint64_t(31)}}));
  BOOST_CHECK("32" == t1.get_events().front()->get_type());
  BOOST_CHECK(1 == t1.get_events().size());
  BOOST_CHECK("32" == t2.get_events().front()->get_type());
  BOOST_CHECK(1 == t2.get_events().size());

  // fire event type 31
  x.propagate_event(core::event::event("31", {{"id", "53"}}));
  BOOST_CHECK(1 == t1.get_events().size());
  BOOST_CHECK(1 == t2.get_events().size());
}


BOOST_AUTO_TEST_CASE(unbind) {
  event_handler_test_class t1;
  event_handler_test_class t2;
  core::event::event_handler x;
  x.bind("32", &t1);
  x.bind("32", &t2);

  // fire event type 32
  x.propagate_event(core::event::event("32", {{"id", std::uint64_t(31)}}));
  BOOST_CHECK("32" == t1.get_events().front()->get_type());
  BOOST_CHECK(1 == t1.get_events().size());
  BOOST_CHECK("32" == t2.get_events().front()->get_type());
  BOOST_CHECK(1 == t2.get_events().size());

  x.unbind("32", &t1);
  x.propagate_event(core::event::event("32", {{"id", std::uint64_t(31)}}));
  BOOST_CHECK("32" == t2.get_events()[1]->get_type());
  BOOST_CHECK(2 == t2.get_events().size());
  BOOST_CHECK(1 == t1.get_events().size());
}


BOOST_AUTO_TEST_CASE(unbind_all) {
  event_handler_test_class t1;
  event_handler_test_class t2;
  core::event::event_handler x;
  x.bind("32", &t1);
  x.bind("32", &t2);
  x.bind("33", &t1);
  x.bind("33", &t2);

  // fire event type 32
  x.propagate_event(core::event::event("32", {{"id", std::uint64_t(31)}}));
  x.propagate_event(core::event::event("33", {{"id", std::uint64_t(31)}}));
  BOOST_CHECK(2 == t1.get_events().size());
  BOOST_CHECK(2 == t2.get_events().size());

  x.unbind(&t1);
  x.propagate_event(core::event::event("32", {{"id", std::uint64_t(31)}}));
  x.propagate_event(core::event::event("33", {{"id", std::uint64_t(31)}}));
  BOOST_CHECK(4 == t2.get_events().size());
  BOOST_CHECK(2 == t1.get_events().size());
}


BOOST_AUTO_TEST_CASE(enqueue_test) {
  event_handler_test_class t1;
  core::event::event_handler x;
  core::event::event e("some_event", {});

  x.bind("some_event", &t1);
  x.enqueue(e, 3);

  BOOST_CHECK(3 == t1.get_events()[0]->get_delay());
}


BOOST_AUTO_TEST_SUITE_END()

