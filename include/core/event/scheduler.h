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


      scheduler() : m_time_delayed_events(PRIORITY::MAX), m_frame_delayed_events(PRIORITY::MAX) {
        init();
      }


      ~scheduler() {
        shutdown();
      }


      void init() {
        m_queued_front_events = new t_event_queue(PRIORITY::MAX);
        m_queued_back_events = new t_event_queue(PRIORITY::MAX);
        for (std::uint32_t i(0); i < PRIORITY::MAX; ++i) {
          (*m_queued_front_events)[i] = new t_event_vector;
          (*m_queued_back_events)[i] = new t_event_vector;
          (*m_queued_front_events)[i]->reserve(MINIMUM_EVENT_RESERVE);
          (*m_queued_back_events)[i]->reserve(MINIMUM_EVENT_RESERVE);
        }

      }


      void shutdown() {
        // delete front
        for (auto& priority_queue : *m_queued_front_events) {
          for (auto e : *priority_queue)
            delete e;
          delete priority_queue;
        }
        m_queued_front_events->clear();

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


      void swap() {
        std::cout << "--- swap start ---" << std::endl;
        // clear front queue
        for (auto& priority_queue : *m_queued_front_events) {
          for (auto e : *priority_queue)
            delete e;
          priority_queue->clear();
        }

        // swap queues
        auto tmp = m_queued_back_events;
        m_queued_back_events = m_queued_front_events;
        m_queued_front_events = tmp;

        // check delayed queues and fill front queue
        std::uint64_t const CURRENT_TIME = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        for (int i(0); i < PRIORITY::MAX; ++i) {
          std::cout << " scanning priority: " << i << std::endl;
          for (auto& timed_priority_pair : m_time_delayed_events[i]) {
            if (timed_priority_pair.first <= CURRENT_TIME) {
              (*m_queued_front_events)[i]->insert(std::end(*(*m_queued_front_events)[i]), std::begin(*timed_priority_pair.second), std::end(*timed_priority_pair.second));
              m_time_delayed_events[i].erase(timed_priority_pair.first);
            }
          }

          auto& priority_frame_map = m_frame_delayed_events[i];
          if (priority_frame_map.end() != priority_frame_map.find(1)) {
            std::cout << "1" << std::endl;
            std::cout << "size: " << priority_frame_map[1]->size() << std::endl;
            (*m_queued_front_events)[i]->insert(std::end(*(*m_queued_front_events)[i]), std::begin(*priority_frame_map[1]), std::end(*priority_frame_map[1]));
            std::cout << "1" << std::endl;
          }
          for (auto& frame_priority_pair : m_frame_delayed_events[i]) {
            std::cout << "2" << std::endl;
            if (frame_priority_pair.first > 1) {
              std::cout << "3" << std::endl;
              m_frame_delayed_events[i][frame_priority_pair.first - 1] = m_frame_delayed_events[i][frame_priority_pair.first];
              m_frame_delayed_events[i].erase(frame_priority_pair.first);
            } else {
              std::cout << "4" << std::endl;
              m_frame_delayed_events[i].erase(frame_priority_pair.first);
            }
          }
        }
        std::cout << "--- swap end ---" << std::endl;
      }


      t_event_vector pull_events() {
        t_event_vector result;

        for (int i(0); i < PRIORITY::MAX; ++i) {
          result.insert(std::end(result), std::begin(*(*m_queued_front_events)[i]), std::end(*(*m_queued_front_events)[i]));
        }

        return result;
      }


      void enqueue(event const& e, std::uint32_t priority = PRIORITY::LOW) {
        if (priority >= PRIORITY::MAX)
          throw std::runtime_error("scheduler::enqueue: priority exceeds priority limits");

        // ensure sufficient capacity without constant reallocation
        auto priority_queue = (*m_queued_back_events)[priority];
        if (priority_queue->size() == priority_queue->capacity()) {
          priority_queue->resize(priority_queue->capacity() << 1);
          (*m_queued_front_events)[priority]->resize(priority_queue->capacity() << 1);
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


      static constexpr std::uint32_t MINIMUM_EVENT_RESERVE =64;

      t_event_queue* m_queued_front_events;
      t_event_queue* m_queued_back_events;
      t_event_map m_time_delayed_events;
      t_event_map m_frame_delayed_events;
    };

  }

}


#endif /* INCLUDE_CORE_EVENT_SCHEDULER_H_ */

