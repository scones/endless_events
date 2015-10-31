/*
 * event_receiver.h
 *
 *  Created on: 29.10.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_EVENT_RECEIVER_H_
#define INCLUDE_CORE_EVENT_RECEIVER_H_


#include "core/event.h"


namespace core {

  class event_receiver {
    typedef std::vector<core::event*> t_event_vector;


    public:


    event_receiver() : m_events() {}
    virtual ~event_receiver() {};


    void receive_event(event const &e) { m_events.push_back(new core::event(e)); }
    t_event_vector const& get_events() const { return m_events; }
    void clear_events() {
      for (auto &e : m_events)
        delete e;
      m_events.clear();
    }


    private:


    t_event_vector m_events;
  };

}


#endif /* INCLUDE_CORE_EVENT_RECEIVER_H_ */

