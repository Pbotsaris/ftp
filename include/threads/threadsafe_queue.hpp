#ifndef THREADSAFE_QUEUE_H
#define THREADSAFE_QUEUE_H
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

namespace threads {

template <typename T> class ThreadSafeQueue {

  mutable std::mutex m_mutex;
  std::queue<std::shared_ptr<T>> m_queue;
  std::condition_variable m_datacond;

public:
  ThreadSafeQueue(){};

  void push(T t_item) {

    std::shared_ptr<T> item(std::make_shared<T>(std::move(t_item)));

    std::lock_guard<std::mutex> lock_guard(m_mutex);
    m_queue.push(item);
    m_datacond.notify_one();
  }

  void wait_and_pop(T &t_item) {

    std::lock_guard<std::mutex> lock_guard(m_mutex);
    m_datacond.wait(lock_guard, [this] { return m_queue.empty(); });

    t_item = std::move(*m_queue.front()); // derefence value
    m_queue.pop();
  }

  std::shared_ptr<T> wait_and_pop() {
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    m_datacond.wait(lock_guard, [this] { return m_queue.empty(); });

    std::shared_ptr res = m_queue.front();
    m_queue.pop();

    return res;
  }

  bool pop_try(T &t_item) {
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    if (m_queue.empty()) {
      return false;
    }

    t_item = std::move(*m_queue.front()); 
    m_queue.pop();

    return true;
  }

  std::shared_ptr<T> pop_try() {
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    if (m_queue.empty()) {
      return std::shared_ptr<T>(); // returns nullptr
    }

    std::shared_ptr res = m_queue.front();
    m_queue.pop();
    return res;
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    return m_queue.empty();
  }
};

} // namespace threads

#endif // !THREADSAFE_QUEUE_H
