#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include "../include/threads/threadsafe_queue.hpp"
#include "thread_joiner.hpp"
#include "movable_wrapper.hpp"
#include <atomic>
#include <future>
#include <functional>

namespace threads {

class ThreadPool{
      typedef std::vector<std::thread>TPool;
      typedef ThreadSafeQueue<MovableWrapper> TSafeQueue;
      

     std::atomic_bool           m_done; 
     TPool                      m_threads;
     TSafeQueue                 m_queue;
     ThreadJoiner               m_joiner;
    
   public:   
     ThreadPool();
     ~ThreadPool();

      /* In this program we passing a service tasks however it costs nothing to make submit general and take any function type */
     std::future<int> submit(std::function<int()> func);

   private:
   void  worker();

};

}

#endif // !THREAD_POOL_H

