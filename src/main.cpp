#include "logger.hpp"
#include "server.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "test.hpp"


int main(void) {

  int res = test();
  if (res > 0){
    return res;
  }

  try {
      auto server = Server(2000);
      server.connect();
      server.main_loop();

  } catch (const char *msg) {
    LOG_ERROR(msg);
  }

  return 0;
}
