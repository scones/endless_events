#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>


#include <cinttypes>
#include <limits>

BOOST_AUTO_TEST_SUITE(general)

BOOST_AUTO_TEST_CASE(check_for_64bit) {
  BOOST_CHECK_MESSAGE(std::uint64_t(std::numeric_limits<std::uint64_t>::max()) > std::numeric_limits<std::uint32_t>::max(), "not enough bits to represent 64 unsigned int");
  BOOST_CHECK_MESSAGE(std::uint64_t(std::numeric_limits<std::uint64_t>::max() + 1) == 0, "2^64 is not 0, 128 bit used?");
  BOOST_CHECK_MESSAGE(8 == sizeof(std::uint64_t), "sizeof std::uint32_t should be 8, was " << sizeof(std::uint64_t));
  BOOST_CHECK_MESSAGE(8 == sizeof(std::int64_t), "sizeof std::int32_t should be 8, was " << sizeof(std::int64_t));
  BOOST_CHECK_MESSAGE(8 == sizeof(void*), "sizeof void* should be 8, was " << sizeof(void*));
  BOOST_CHECK_MESSAGE(8 == sizeof(double), "sizeof float should be 8, was " << sizeof(float));
}

BOOST_AUTO_TEST_SUITE_END()

