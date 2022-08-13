#include "logger.hpp"
#include "server.hpp"
#include "arg.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "test.hpp"


int main(int ac, char **av) {

  int res = 0;

  if ((res = test()) > 0) { return res; } // tests
  
  int port = Arg::get_port_when_valid(ac, av); // validate and extract arguments

  if(Arg::invalid_port(port)){
     return 1;
  }

  try {
    auto server = Server(port);
    server.connect();
    server.main_loop();

  } catch (const char *msg) {
    LOG_ERROR(msg);
  }

  return res;
}
