#ifndef PARSER_H
#include <iostream>
#define PARSER_H
#include "commands.hpp"
#include "utils.hpp"
#include "request.hpp"
#include "logger.hpp"

namespace parsing {

struct Parser {
  struct Private;
  static void parse(networking::Request &t_req);

private:
  static void parse_command(networking::Request &t_req, utils::StringVector t_commands);
  static void parse_argument(networking::Request &t_req, utils::StringVector t_commands);
};

} // namespace parsing

#endif
