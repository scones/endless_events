/*
 * event_test.cpp
 *
 *  Created on: 31.10.2015
 *      Author: scones
 */


#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>


#include "core/event/event.h"


BOOST_AUTO_TEST_SUITE(event_test)


BOOST_AUTO_TEST_CASE(getter) {
  core::event::event e(
    "1",
    {
      {"id", std::uint64_t(512)},
      {"time", std::uint64_t(12345678901)}
    }
  );

  BOOST_CHECK("1" == e.get_type());
  BOOST_CHECK(std::uint64_t(512) == e["id"]);
}


BOOST_AUTO_TEST_SUITE_END()

