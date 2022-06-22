#include "../../include/server.h"

int main (void)
{
  auto server = Server(4000, 5);

  server.set_socket_options();
  server.config_addr();
  server.bind_socket();
  server.slisten();
  server.receive();
}
