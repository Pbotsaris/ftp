#include "../../include/service.h"

int main(void) {
  try {
    auto server = Service(4000);
    server.setup();
    server.main_loop();

  } catch (const char *msg) {

    std::cout << msg << "\n";

  }

  return 0;
}
