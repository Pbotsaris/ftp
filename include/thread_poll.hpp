#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include"threadsafe_queue.hpp"
#include <vector>
#include <atomic>


//const int MAX_CURRENCY = std::thread::hardware_concurrency();

class ThreadPool{
     typedef std::vector<std::thread>TPool;
      typedef ThreadSafeQueue<std::function<void()>> TSafeQueue;

     TPool                      m_treadhpool;
     TSafeQueue                 m_queue;
     std::atomic_bool           done; 
    
   public:   
     ThreadPool();
     ~ThreadPool();

};

#endif // !THREAD_POOL_H

