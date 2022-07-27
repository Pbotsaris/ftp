#include "doctest.h"
#include <future>
#include <iostream>
#include "movable_wrapper.hpp"
#include "thread_pool.hpp"
#include "../include/threads/threadsafe_queue.hpp"

using namespace threads;

ThreadPool::ThreadPool() : m_done(false), m_joiner(m_threads) {

  const std::size_t thread_count = std::thread::hardware_concurrency();

  try {
    for (std::size_t i = 0; i < thread_count; i++) {

      m_threads.push_back(std::thread(&ThreadPool::worker, this));
    }
    /* create ThreadPool.*/

  } catch (...) {
    m_done = true;
    throw "Could not initialize ThreadPool.";
  }
}

ThreadPool::~ThreadPool() { m_done = true; }

void ThreadPool::worker() {

  while (!m_done) {

    MovableWrapper service_task;

    /* grab pending service from queue to work on a thread */
    if (m_queue.pop_try(service_task)) {
      service_task();
    } else {
      std::this_thread::yield();
    }
  }
}

// using type traits for a make future type returns dynamic
std::future<int> ThreadPool::submit(std::function<int()> t_func) {

  std::packaged_task<int()> service_task(std::move(t_func));

  std::future<int> future(service_task.get_future());
  m_queue.push(std::move(service_task));

  return future;
}
