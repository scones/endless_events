/*
 * event_handler.h
 *
 *  Created on: 29.10.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_EVENT_EVENT_HANDLER_H_
#define INCLUDE_CORE_EVENT_EVENT_HANDLER_H_


#include "event_receiver.h"
#include "event.h"


#include <stdexcept>
#include <iostream>


namespace core {

  namespace event {

    class event_handler {

      public:


      event_handler() {}
      ~event_handler() {}


      void bind(std::string const& type, core::event::event_receiver* object) {
        m_registries[type].push_back(object);
      }


      void unbind(std::string const& type, core::event::event_receiver* object) {
        auto& receivers = m_registries[type];
        for (auto it = receivers.begin(); it != receivers.end(); ++it) {
          if (*it == object) {
            receivers.erase(it);
          }
        }
      }


      void unbind(core::event::event_receiver * object) {
        for (auto& pair : m_registries) {
          for (auto it = pair.second.begin(); it != pair.second.end(); ++it) {
            if (*it == object) {
              pair.second.erase(it);
            }
          }
        }
      }


      void propagate_event(event const& e) {
        try {
          auto receivers = m_registries.at(e.get_type());
          for (auto receiver : receivers)
            receiver->receive_event(e);
        } catch (std::out_of_range &oor) {
        }
      }


      protected:


      typedef std::unordered_map<std::string, std::vector<event_receiver *>> t_event_receiver_vector_map;
      t_event_receiver_vector_map m_registries;
    };

  }

}


#endif /* INCLUDE_CORE_EVENT_EVENT_HANDLER_H_ */

