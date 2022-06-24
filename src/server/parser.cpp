#include "../../include/parser.h"
#include <exception>

using namespace parsing;

const int COMMAND = 0;
const int ARGUMENT = 1;

struct Parser::Private {

  static StringVector split_string(std::string &s, std::string del = " ") {
    int start = 0;
    int end = s.find(del);
    StringVector result;

    while (end != -1) {
      result.push_back(s.substr(start, end - start));
      start = end + del.size();
      end = s.find(del, start);
    }
    result.push_back(s.substr(start, end - start));

    return result;
  }

  static void parse_command(networking::Request &t_req, StringVector t_commands) {
    try {
      t_req.m_command = commands::find.at(t_commands[COMMAND]);

    } catch (std::exception &err) {
      t_req.m_reply = networking::reply::r_502;
      t_req.m_valid = false;
    };
  }

  static void parse_argument(networking::Request &t_req, StringVector t_commands) {
    try {
      t_req.m_argument = t_commands.at(ARGUMENT);

    } catch (std::exception &err) {

      t_req.m_reply = networking::reply::r_501;
      t_req.m_valid = false;
    }
  }
};

void Parser::parse(networking::Request &t_req) {

  StringVector split_command = Private::split_string(t_req.m_raw);
  Private::parse_command(t_req, split_command);

  commands::type type = commands::Utils::get_type(t_req.m_command);

  if (!t_req.m_valid || type == commands::type::no_argument)
    return;

  Private::parse_argument(t_req, split_command);
}
