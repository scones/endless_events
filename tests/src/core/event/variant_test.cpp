/*
 * variant_test.cpp
 *
 *  Created on: 30.10.2015
 *      Author: scones
 */


#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>


#include "core/event/variant.h"


#include <cstring>


BOOST_AUTO_TEST_SUITE(variant_test)


BOOST_AUTO_TEST_CASE(getter) {
  BOOST_CHECK(std::uint64_t(1) == core::event::variant(std::uint64_t(1)).get_uint_value());
  BOOST_CHECK(std::int64_t(-1) == core::event::variant(std::int64_t(-1)).get_int_value());
  BOOST_CHECK(double(1.2e16) == core::event::variant(double(1.2e16)).get_double_value());
  BOOST_CHECK((void*)(this) == core::event::variant((void*)(this)).get_pointer_value());
  BOOST_CHECK(0 == std::strcmp("foo bar", core::event::variant("foo bar").get_string_value()));
  BOOST_CHECK(core::event::variant::TYPE::INT == core::event::variant(std::int64_t(-3)).get_type());
}


BOOST_AUTO_TEST_CASE(not_operator) {
  BOOST_CHECK(false == !core::event::variant(true));
  BOOST_CHECK(!core::event::variant(false));
}


BOOST_AUTO_TEST_CASE(direct_compare) {
  BOOST_CHECK(core::event::variant(std::uint64_t(1)) == core::event::variant(std::uint64_t(1)));
  BOOST_CHECK(!(core::event::variant(std::uint64_t(1)) == core::event::variant(std::uint64_t(2))));
}


BOOST_AUTO_TEST_CASE(bool_compare) {
  BOOST_CHECK(true == core::event::variant(true));
  BOOST_CHECK(false == core::event::variant(false));
}


BOOST_AUTO_TEST_CASE(less_than) {
  BOOST_CHECK(std::uint64_t(1) < core::event::variant(std::uint64_t(2)));
}


BOOST_AUTO_TEST_CASE(greater_than) {
  BOOST_CHECK(std::uint64_t(2) > core::event::variant(std::uint64_t(1)));
}


BOOST_AUTO_TEST_CASE(setter) {
  core::event::variant x(std::uint64_t(1));

  x = std::uint64_t(2);
  BOOST_CHECK(2 == x.get_uint_value());

  x = std::int64_t(-1);
  BOOST_CHECK(-1 == x.get_int_value());

  x = false;
  BOOST_CHECK(false == x.get_bool_value());

  x = 1.2e16;
  BOOST_CHECK(1.2e16 == x.get_double_value());

  x = "meh";
  BOOST_CHECK(0 == std::strcmp("meh", x.get_string_value()));
}


// for use with containers this should be possible
BOOST_AUTO_TEST_CASE(blank_init) {
  core::event::variant x;
  BOOST_CHECK(core::event::variant::TYPE::UINT == x.get_type());
  BOOST_CHECK(std::uint64_t(0) == x.get_uint_value());
}


BOOST_AUTO_TEST_SUITE_END()

