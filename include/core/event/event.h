/*
 * event.h
 *
 *  Created on: 29.10.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_EVENT_EVENT_H_
#define INCLUDE_CORE_EVENT_EVENT_H_


#include "variant.h"


#include <unordered_map>
#include <string>
#include <initializer_list>
#include <cinttypes>


namespace core {

  namespace event {

    class event {
      typedef std::unordered_map<std::string, core::event::variant> t_variant_map;


      public:


      event(std::string const& type, std::initializer_list<t_variant_map::value_type> const& args, std::uint32_t delay = 0) : c_type(type), m_delay(delay), m_argument_map(args) {}
      ~event() {}

      core::event::variant const& operator[](std::string const& name) { return m_argument_map[name]; }
      core::event::variant const& get_argument(std::string const& name) { return m_argument_map[name]; }
      std::string const get_type() const { return c_type; }

      void set_delay(std::uint32_t delay) { m_delay = delay; }
      std::uint32_t get_delay() const { return m_delay; };


      protected:


      std::string const c_type;
      std::uint32_t m_delay;
      t_variant_map m_argument_map;
    };

  }

}


#endif /* INCLUDE_CORE_EVENT_EVENT_H_ */
