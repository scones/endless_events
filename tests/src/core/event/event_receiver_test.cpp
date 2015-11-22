/*
 * event_receiver_test.cpp
 *
 *  Created on: 31.10.2015
 *      Author: scones
 */


#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>


#include "core/event/event_receiver.h"


class event_receiver_test_class : public core::event::event_receiver {

  public:

  event_receiver_test_class() : core::event::event_receiver() {}
  ~event_receiver_test_class() {};

};


BOOST_AUTO_TEST_SUITE(event_receiver_test)


BOOST_AUTO_TEST_CASE(main_event_propagate) {
  event_receiver_test_class x;

  BOOST_CHECK(0 == x.get_events().size());

  core::event::event y(std::uint64_t(12), {{"id", std::uint64_t(123)}});
  x.receive_event(y);
  BOOST_CHECK(1 == x.get_events().size());
  BOOST_CHECK(12 == x.get_events().front()->get_type());
  BOOST_CHECK(123 == x.get_events().front()->get_argument("id"));

  x.clear_events();
  BOOST_CHECK(0 == x.get_events().size());
}


BOOST_AUTO_TEST_SUITE_END()

