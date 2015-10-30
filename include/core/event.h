/*
 * event.h
 *
 *  Created on: 29.10.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_EVENT_H_
#define INCLUDE_CORE_EVENT_H_


#include "core/variant.h"


#include <unordered_map>
#include <string>
#include <initializer_list>
#include <cinttypes>


namespace core {

  class event {
    typedef std::unordered_map<std::string, core::variant> t_variant_map;


    public:


    event(std::uint32_t type, std::initializer_list<t_variant_map::value_type> const& args) : c_type(type), m_argument_map(args) {}
    ~event() {}

    core::variant const& operator[](std::string const& name) { return m_argument_map[name]; }
    std::uint32_t const get_type() const { return c_type; }


    protected:


    std::uint32_t const c_type;
    t_variant_map m_argument_map;
  };

}


#endif /* INCLUDE_CORE_EVENT_H_ */
