/*
 * event_handler.h
 *
 *  Created on: 29.10.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_EVENT_HANDLER_H_
#define INCLUDE_CORE_EVENT_HANDLER_H_


#include "event_receiver.h"


namespace core {

  class event_handler {
    typedef std::unordered_map<std::uint32_t, std::vector<event const *>> t_event_vector_map;


    event_handler() {}
    ~event_handler() {}

    void bind(std::uint32_t const type, core::event_receiver* const object) {
      m_registries[type].push_back(object);
    }

    void unbind(std::uint32_t const type, core::event_receiver const * const object) {
      auto& pair = m_registries[type];
      for (auto it = pair.second().begin(); it != pair.second().end(); ++it) {
        if (*it == object) {
          pair.second().erase(it);
        }
      }
    }

    void unbind(core::event_receiver const * const object) {
      for (auto& pair : m_registries) {
        for (auto it = pair.second().begin(); it != pair.second().end(); ++it) {
          if (*it == object) {
            pair.second().erase(it);
          }
        }
      }
    }


    protected:


    t_event_vector_map m_registries;
  };

}


#endif /* INCLUDE_CORE_EVENT_HANDLER_H_ */

