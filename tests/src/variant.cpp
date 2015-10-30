/*
 * variant.cpp
 *
 *  Created on: 30.10.2015
 *      Author: scones
 */


#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>


#include "../../include/core/variant.h"


BOOST_AUTO_TEST_SUITE(variant)


BOOST_AUTO_TEST_CASE(getter) {
  BOOST_CHECK(std::uint64_t(1) == core::variant(std::uint64_t(1)).get_uint_value());
  BOOST_CHECK(std::int64_t(-1) == core::variant(std::int64_t(-1)).get_int_value());
  BOOST_CHECK(double(1.2e16) == core::variant(double(1.2e16)).get_double_value());
  BOOST_CHECK((void*)(this) == core::variant((void*)(this)).get_pointer_value());
}


BOOST_AUTO_TEST_CASE(not_operator) {
  BOOST_CHECK(false == !core::variant(true));
  BOOST_CHECK(!core::variant(false));
}


BOOST_AUTO_TEST_CASE(direct_compare) {
  BOOST_CHECK(core::variant(std::uint64_t(1)) == core::variant(std::uint64_t(1)));
  BOOST_CHECK(!(core::variant(std::uint64_t(1)) == core::variant(std::uint64_t(2))));
}


BOOST_AUTO_TEST_CASE(bool_compare) {
  BOOST_CHECK(true == core::variant(true));
  BOOST_CHECK(false == core::variant(false));
}


BOOST_AUTO_TEST_CASE(less_than) {
  BOOST_CHECK(std::uint64_t(1) < core::variant(std::uint64_t(2)));
}


BOOST_AUTO_TEST_CASE(greater_than) {
  BOOST_CHECK(std::uint64_t(2) > core::variant(std::uint64_t(1)));
}


BOOST_AUTO_TEST_CASE(setter) {
  core::variant x(std::uint64_t(1));

  x = std::uint64_t(2);
  BOOST_CHECK(2 == x.get_uint_value());

  x = std::int64_t(-1);
  BOOST_CHECK(-1 == x.get_int_value());

  x = false;
  BOOST_CHECK(false == x.get_bool_value());

  x = 1.2e16;
  BOOST_CHECK(1.2e16 == x.get_double_value());
}


BOOST_AUTO_TEST_SUITE_END()

