/*
 * scheduler.h
 *
 *  Created on: 14 Feb 2016
 *      Author: scones
 */

#ifndef INCLUDE_CORE_EVENT_SCHEDULER_H_
#define INCLUDE_CORE_EVENT_SCHEDULER_H_


#include "core/event/event.h"


#include <cinttypes>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <type_traits>


namespace core {

  namespace event {

    class scheduler {

      public:


      enum PRIORITY {
        IMMEDIATE = 0,
        HIGH,
        MEDIUM,
        LOW,
        MAX
      };

      typedef std::vector<event*> t_event_vector;
      typedef std::vector<t_event_vector*> t_event_queue;
      typedef std::vector<std::unordered_map<std::uint64_t, t_event_vector*>> t_event_map;

      using event_callback_function = std::add_pointer<void(event const&)>::type;


      scheduler()
        :
          m_front_events(),
          m_queued_back_events(),
          m_time_delayed_events(PRIORITY::MAX),
          m_frame_delayed_events(PRIORITY::MAX),
          m_reserve_level(1),
          m_event_callback_function(nullptr)
      {
        init();
      }


      ~scheduler() {
        shutdown();
      }


      void init() {
        m_queued_back_events = new t_event_queue(PRIORITY::MAX);
        for (std::uint32_t i(0); i < PRIORITY::MAX; ++i) {
          (*m_queued_back_events)[i] = new t_event_vector;
          (*m_queued_back_events)[i]->reserve(MINIMUM_EVENT_RESERVE << m_reserve_level);
        }
        m_front_events.reserve(MINIMUM_EVENT_RESERVE << (m_reserve_level + 2) );
      }


      void shutdown() {
        // delete front
        for (auto& front_event : m_front_events) {
          delete front_event;
        }
        m_front_events.clear();

        // delete back
        for (auto& priority_queue : *m_queued_back_events) {
          for (auto e : *priority_queue)
            delete e;
          delete priority_queue;
        }
        m_queued_back_events->clear();

        // delete time delayed events
        for (auto& priority_map : m_time_delayed_events) {
          for (auto& priority_time_pair : priority_map) {
            for (auto& e : *(priority_time_pair.second))
              delete e;
            priority_map.clear();
          }
          priority_map.clear();
        }

        // delete frame delayed events
        for (auto& priority_map : m_frame_delayed_events) {
          for (auto& priority_frame_pair : priority_map) {
            for (auto& e : *(priority_frame_pair.second))
              delete e;
            priority_map.clear();
          }
          priority_map.clear();
        }
      }


      void increase_reserve_level() {
        ++m_reserve_level;
        for (std::uint32_t i(0); i < PRIORITY::MAX; ++i) {
          (*m_queued_back_events)[i]->reserve(MINIMUM_EVENT_RESERVE << m_reserve_level);
        }
        m_front_events.reserve(MINIMUM_EVENT_RESERVE << (m_reserve_level + 2) );
      }


      void set_event_callback(event_callback_function func) {
        m_event_callback_function = func;
      }


      void swap() {
        // clear front queue
        for (auto& front_event : m_front_events)
          delete front_event;
        m_front_events.clear();

        // check delayed queues and fill front queue
        std::uint32_t extra_event_count = 0;
        std::uint64_t const CURRENT_TIME = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        for (int i(0); i < PRIORITY::MAX; ++i) {

          // copy from basic back events
          m_front_events.insert(std::end(m_front_events), std::begin(*((*m_queued_back_events)[i])), std::end(*((*m_queued_back_events)[i])));
          (*m_queued_back_events)[i]->clear();

          // copy elapsed time events
          for (auto& timed_priority_pair : m_time_delayed_events[i]) {
            if (timed_priority_pair.first <= CURRENT_TIME) {
              extra_event_count += timed_priority_pair.second->size();
              m_front_events.insert(std::end(m_front_events), std::begin(*timed_priority_pair.second), std::end(*timed_priority_pair.second));
              m_time_delayed_events[i].erase(timed_priority_pair.first);
            }
          }

          // copy elasped frame events
          auto& priority_frame_map = m_frame_delayed_events[i];
          if (priority_frame_map.end() != priority_frame_map.find(1)) {
            extra_event_count += priority_frame_map[1]->size();
            m_front_events.insert(std::end(m_front_events), std::begin(*priority_frame_map[1]), std::end(*priority_frame_map[1]));
          }
          for (auto& frame_priority_pair : m_frame_delayed_events[i]) {
            if (frame_priority_pair.first > 1) {
              m_frame_delayed_events[i][frame_priority_pair.first - 1] = m_frame_delayed_events[i][frame_priority_pair.first];
              m_frame_delayed_events[i].erase(frame_priority_pair.first);
            } else {
              m_frame_delayed_events[i].erase(frame_priority_pair.first);
            }
          }
        }

        if (MINIMUM_EVENT_RESERVE << m_reserve_level < extra_event_count)
          increase_reserve_level();
      }


      t_event_vector* poll_events() {
        if (m_event_callback_function) {
          for (auto front_event : m_front_events) {
            m_event_callback_function(*front_event);
          }
          return nullptr;
        } else {
          return &m_front_events;
        }
      }


      void enqueue(event const& e, std::uint32_t priority = PRIORITY::LOW) {
        if (priority >= PRIORITY::MAX)
          throw std::runtime_error("scheduler::enqueue: priority exceeds priority limits");

        // ensure sufficient capacity without constant reallocation
        auto& priority_queue = (*m_queued_back_events)[priority];
        if (priority_queue->size() == priority_queue->capacity()) {
          increase_reserve_level();
          priority_queue->resize(priority_queue->capacity() << 1);
        }
        priority_queue->push_back(new core::event::event(e));
      }


      void enqueue_time_delayed(event const& e, std::uint32_t delay_in_milliseconds, std::uint32_t priority = PRIORITY::LOW) {
        if (priority >= PRIORITY::MAX)
          throw std::runtime_error("scheduler::enqueue_time_delayed: priority exceeds priority limits");

        //get current unix timestamp in milliseconds
        std::uint64_t current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        std::uint64_t delayed_time = current_time + delay_in_milliseconds;

        auto& priority_time_map = m_time_delayed_events[priority];
        auto it = priority_time_map.find(delayed_time);
        if (it == priority_time_map.end()) {
          priority_time_map[delayed_time] = new t_event_vector;
        }
        priority_time_map[delayed_time]->push_back(new core::event::event(e));
      }


      void enqueue_frame_delayed(event const& e, std::uint32_t delay_in_frames, std::uint32_t priority = PRIORITY::LOW) {
        if (priority >= PRIORITY::MAX)
          throw std::runtime_error("scheduler::enqueue_frame_delayed: priority exceeds priority limits");

        auto& priority_frame_map = m_frame_delayed_events[priority];
        auto it = priority_frame_map.find(delay_in_frames);
        if (it == priority_frame_map.end()) {
          priority_frame_map[delay_in_frames] = new t_event_vector;
        }
        priority_frame_map[delay_in_frames]->push_back(new core::event::event(e));
      }


       protected:


      static constexpr std::uint64_t MINIMUM_EVENT_RESERVE = 64;


      t_event_vector m_front_events;
      t_event_queue* m_queued_back_events;
      t_event_map m_time_delayed_events;
      t_event_map m_frame_delayed_events;
      std::uint64_t m_reserve_level;
      event_callback_function m_event_callback_function;
    };

  }

}


#endif /* INCLUDE_CORE_EVENT_SCHEDULER_H_ */

