#include "thread_joiner.hpp"

using namespace threads;

ThreadJoiner::ThreadJoiner(std::vector<std::thread> &t_threads)
    : m_threads(t_threads) {}

ThreadJoiner::~ThreadJoiner() {

  for (std::size_t i= 0; i < m_threads.size(); i++) {
    if (m_threads.at(i).joinable()) {
      m_threads.at(i).join();
    }
  }
}
