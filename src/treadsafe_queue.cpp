#include "threadsafe_queue.hpp"
#include <memory>

template <typename T>
ThreadSafeQueue<T>::ThreadSafeQueue() {}

template <typename T>
void ThreadSafeQueue<T>::push(T t_item) {
  std::shared_ptr<T> item(std::make_shared<T>(std::move(t_item)));

  std::lock_guard<std::mutex> lock_guard(m_mutex);
  m_queue.push(item);
  m_datacond.notify_one();
}

template <typename T>
void ThreadSafeQueue<T>::wait_and_pop(T &t_item) {
  std::lock_guard<std::mutex> lock_guard(m_mutex);
  m_datacond.wait(lock_guard, [this] { return m_queue.empty(); });

  t_item = std::move(*m_queue.front()); // derefence value
  m_queue.pop();
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::wait_and_pop() {
  std::lock_guard<std::mutex> lock_guard(m_mutex);
  m_datacond.wait(lock_guard, [this] { return m_queue.empty(); });

  std::shared_ptr res = m_queue.front();
  m_queue.pop();

  return res;
}

template <typename T>
bool ThreadSafeQueue<T>::pop_try(T &t_item) {
  std::lock_guard<std::mutex> lock_guard(m_mutex);
  if (m_queue.empty()) {
    return false;
  }
  m_queue.pop();

  return true;
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::pop_try() {
  std::lock_guard<std::mutex> lock_guard(m_mutex);
  if (m_queue.empty()) {
    return std::shared_ptr<T>(); // returns nullptr
  }

  std::shared_ptr res = m_queue.front();
  m_queue.pop();
  return res;
}

template <typename T>
bool ThreadSafeQueue<T>::empty() const {
  std::lock_guard<std::mutex> lock_guard(m_mutex);
  return m_queue.empty();
}
