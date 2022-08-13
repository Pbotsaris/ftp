#include "test.hpp"

int test() {
  doctest::Context ctx;
  ctx.setOption("abort-after", 5);
  ctx.setOption("no-breaks", true);

  int res = ctx.run();

  return res;

  if (ctx.shouldExit())
    return -1;
}

