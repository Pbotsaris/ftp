#include "../../include/parser.h"
#include "../../include/reply.hpp"
#include "../../include/request.h"
#include "../../include/service.h"
#include "../../include/users.h"
#include "../../include/logger.hpp"
#include <tuple>


int main(void) {

 try {
   auto service = networking::Service(4000);
   service.setup();
   service.handshake();
   service.main_loop();

 } catch (const char *msg) {
   LOG_ERROR(msg);
 }

  return 0;
}
