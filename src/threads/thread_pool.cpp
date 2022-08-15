#include "thread_pool.hpp"
#include "../include/threads/threadsafe_queue.hpp"
#include "doctest.h"
#include "movable_wrapper.hpp"
#include <future>
#include <iostream>

using namespace threads;

static int ITERATIONS_UNTIL_THROTTLE = 40;

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

  int throttle_count = ITERATIONS_UNTIL_THROTTLE;

  while (!m_done) {
    MovableWrapper service_task;

    /* grab pending service from queue to work on a thread */
    if (m_queue.pop_try(service_task)) {
      throttle_count = ITERATIONS_UNTIL_THROTTLE;
      ;
      service_task();
    } else {
      std::this_thread::yield();
      countdown(throttle_count);
      throttle(throttle_count); // wait no tasks in queue
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

void ThreadPool::countdown(int &t_count) {
  if (t_count > 0) {
    t_count--;
  }
}

void ThreadPool::throttle(const int t_count) {

  if (t_count == 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  else if (t_count > 0 && t_count <= 20) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  else if (t_count > 20 && t_count <= 40) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}
