#ifndef PARSER_H
#define PARSER_H
#include "commands.hpp"
#include "request.h"
#include <iostream>

namespace parsing {

typedef std::vector<std::string> StringVector;

class Parser {
  struct Private;

public:

  void parse(networking::Request &t_req);
};

} // namespace parsing

#endif
