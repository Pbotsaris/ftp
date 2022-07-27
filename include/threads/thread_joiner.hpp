
#ifndef THREAD_JOINER_H
#define THREAD_JOINER_H
#include <vector>
#include <thread>

/* 
 * this class will join a a vector of threads when its object runs out of scope. 
 *
 * */

namespace threads {
class ThreadJoiner{
  std::vector<std::thread> &m_threads;

  public:
  explicit ThreadJoiner(std::vector<std::thread> &t_threads);

  ~ThreadJoiner();

};
}

#endif // !THREAD_JOINER_H
