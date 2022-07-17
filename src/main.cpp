#include "parser.hpp"
#include "reply.hpp"
#include "request.hpp"
#include "service.hpp"
#include "accounts.hpp"
#include "logger.hpp"
#include "server.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "../include/doctest.h"

int test()
{
 doctest::Context ctx;
 ctx.setOption("abort-after", 5);
 ctx.setOption("no-breaks", true);

 int res = ctx.run();

 if(ctx.shouldExit())
     return res;

  return -1;
}

int main(void) {

//      int res = test();
//      if(res > 0)
//         return res;
//
//
 try {
     auto server = Server(2000);
     server.new_connection();
     server.main_loop();
 //  auto service = networking::Service(2000);
 //  service.control_setup();
 //  service.control_handshake();
 //  service.control_loop();


 } catch (const char *msg) {
     LOG_ERROR(msg);
 }

  return 0;
}

