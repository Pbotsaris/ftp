#include "../../include/service.h"
#include "../../include/parser.h"
#include "../../include/request.h"
#include "../../include/reply.hpp"

int main(void) {
 
  try {
    auto server = networking::Service(4000);
    server.setup();
    server.handshake();
    server.main_loop();

  } catch (const char *msg) {
    std::cout << msg << "\n";
  }

  return 0;
}
