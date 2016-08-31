/*
 * event.h
 *
 *  Created on: 29.10.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_EVENT_EVENT_H_
#define INCLUDE_CORE_EVENT_EVENT_H_


#include <unordered_map>
#include <string>
#include <initializer_list>
#include <cinttypes>


#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/constructible.hpp>
#include <boost/mpl/vector.hpp>


namespace core {

  namespace event {

    class event {
      typedef boost::type_erasure::any<
        boost::mpl::vector<
          boost::type_erasure::constructible<boost::type_erasure::_self()>,
          boost::type_erasure::copy_constructible<>,
          boost::type_erasure::equality_comparable<>,
          boost::type_erasure::less_than_comparable<>,
          boost::type_erasure::ostreamable<>,
          boost::type_erasure::typeid_<>,
//          boost::type_erasure::istreamable<>,
          boost::type_erasure::relaxed
        >
      > t_variant;
      typedef std::unordered_map<std::string, t_variant> t_attributes_map;


      public:


      event(std::string const& type, std::initializer_list<t_attributes_map::value_type> const& args) : c_type(type), m_argument_map(args) {}
      ~event() {}

      t_variant const& operator[](std::string const& name) { return m_argument_map[name]; }
      t_variant const& get_argument(std::string const& name) { return m_argument_map[name]; }
      std::string const get_type() const { return c_type; }


      protected:


      std::string const c_type;
      t_attributes_map m_argument_map;
    };

  }

}


#endif /* INCLUDE_CORE_EVENT_EVENT_H_ */

