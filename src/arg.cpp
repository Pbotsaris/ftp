#include "arg.hpp"
#include "logger.hpp"
#include <iostream>

const int Arg::MIN_ARG_NUM = 2;
const int Arg::MIN_PORT_NUM = 2000;
const int Arg::MAX_PORT_NUM = 40000;

int Arg::get_port_when_valid(int ac, char **av) {

  int port = -1;

  if (MIN_ARG_NUM > ac) {
    LOG_ERROR("You must pass in the port as the first argument to run the server -> ./ftp 4000");
    return -1;
  }

  try {
    port = std::stoi(av[1]);
  } catch (...) {

    LOG_ERROR("Port must be a valid integer. got: %s . Exiting...", av[1]);
    return -1;
  }

  if (port >= MIN_PORT_NUM && port <= MAX_PORT_NUM) {
    return port;
  } else {

    LOG_ERROR("Port numbert must bet between %i and %i. You port was: %s. Exiting ...",
              MIN_PORT_NUM, MAX_PORT_NUM, av[1]);
    return -1;
  }
}

bool Arg::invalid_port(int port){
    return port < 0;
}
