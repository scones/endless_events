/*
 * variant.h
 *
 *  Created on: 29.10.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_VARIANT_H_
#define INCLUDE_CORE_VARIANT_H_


#include <cinttypes>


namespace core {

  class variant {

    public:


    enum TYPE {
      STRING_ID = 1,
      UINT,
      INT,
      DOUBLE,
      BOOL,
      POINTER,
      MAX
    };

    variant(std::uint64_t const value) : m_type(UINT), m_uint_value(value) {}
    variant(std::int64_t const value) : m_type(INT), m_int_value(value) {}
    variant(double const value) : m_type(DOUBLE), m_double_value(value) {}
    variant(bool const value) : m_type(BOOL), m_uint_value((std::uint64_t)value) {}
    variant(void* const value) : m_type(POINTER), m_pointer_value(value) {}

    variant(variant const& rhs) = default;
    variant(variant&& rhs) = default;

    void operator=(std::uint64_t const value) { m_uint_value = value; }
    void operator=(std::int64_t const value) { m_int_value = value; }
    void operator=(double const value) { m_double_value = value; }
    void operator=(bool const value) { m_uint_value = (std::uint64_t)value; }

    bool operator!() { return 0 == m_uint_value; }
    friend bool operator==(variant const& lhs, variant const& rhs) { return lhs.m_uint_value == rhs.m_uint_value; }
    friend bool operator==(bool lhs, variant const& rhs) { return lhs == (bool)rhs.m_uint_value; }
    friend bool operator==(variant const& lhs, bool rhs) { return (bool)lhs.m_uint_value == rhs; }
    friend bool operator<(variant const& lhs, variant const& rhs) { return lhs.m_uint_value < rhs.m_uint_value; }
    friend bool operator>(variant const& lhs, variant const& rhs) { return lhs.m_uint_value > rhs.m_uint_value; }

    std::uint64_t const get_uint_value() const { return m_uint_value; }
    std::int64_t const get_int_value() const { return m_int_value; }
    double const get_double_value() const { return m_double_value; }
    bool const get_bool_value() const { return 0 != m_uint_value; }
    void* const get_pointer_value() const { return m_pointer_value; }


    protected:


    TYPE m_type;

    union {
      std::uint64_t m_uint_value;
      std::int64_t m_int_value;
      double m_double_value;
      void* const m_pointer_value;
    };

  };

}


#endif /* INCLUDE_CORE_VARIANT_H_ */

