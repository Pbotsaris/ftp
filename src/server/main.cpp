#include "../../include/parser.h"
#include "../../include/reply.hpp"
#include "../../include/request.h"
#include "../../include/service.h"
#include "../../include/accounts.h"
#include "../../include/logger.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "../../include/doctest.h"

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

      int res = test();
      if(res > 0)
         return res;

 try {
   auto service = networking::Service(4000);
   service.control_setup();
   service.control_handshake();
   service.control_loop();

 } catch (const char *msg) {
   LOG_ERROR(msg);
 }

  return 0;
}


//  Class connection. 
//
//  Class Service ->  2 connections  1. control  / 2.Data
