#ifndef PARSER_H
#define PARSER_H
#include "commands.hpp"
#include "request.h"
#include <iostream>

namespace parsing {

class Parser {
  struct Private;

public:

  void parse(networking::Request &t_req);
};

} // namespace parsing

#endif
