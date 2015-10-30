/*
 * event_receiver.h
 *
 *  Created on: 29.10.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_EVENT_RECEIVER_H_
#define INCLUDE_CORE_EVENT_RECEIVER_H_


namespace core {

  class event_receiver {
    typedef std::vector<core::event const*> t_event_vector;


    public:


    event_receiver() : m_events() {}

    void event_receive(event const *e) { m_events.push_back(e); }
    t_event_vector const& event_get_events() const { return m_events; }
    void event_clear_events() { m_events.clear(); }


    private:


    t_event_vector m_events;
  };

}


#endif /* INCLUDE_CORE_EVENT_RECEIVER_H_ */

