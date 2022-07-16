#include "thread_poll.hpp"
#include "doctest.h"
#include <iostream>

ThreadPool::ThreadPool(){

  const int num_threads = std::thread::hardware_concurrency();



}


TEST_CASE("Testing threads"){

 int num_threads = std::thread::hardware_concurrency();
 std::cout << num_threads << std::endl;

}
