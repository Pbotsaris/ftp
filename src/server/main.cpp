#include "../../include/parser.h"
#include "../../include/reply.hpp"
#include "../../include/request.h"
#include "../../include/service.h"
#include "../../include/users.h"
#include "../../include/logger.hpp"
#include <tuple>


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
