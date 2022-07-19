
#ifndef THREADSAFE_QUEUE_H
#define THREADSAFE_QUEUE_H
#include <thread>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>

#include <functional>


template<typename T>
class ThreadSafeQueue {

  mutable std::mutex              m_mutex;
  std::queue<std::shared_ptr<T>>  m_queue;
  std::condition_variable         m_datacond; 

  public:
   ThreadSafeQueue();

   void               push(T t_item);
   void               wait_and_pop(T &t_item);
   std::shared_ptr<T> wait_and_pop();

   bool               pop_try(T &t_item);
   std::shared_ptr<T> pop_try();

   bool               empty() const;



};


#endif // !THREADSAFE_QUEUE_H
