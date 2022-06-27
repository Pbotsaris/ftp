#include "../../include/parser.h"
#include <exception>

using namespace parsing;

static const int COMMAND = 0;
static const int ARGUMENT = 1;

void Parser::parse_command(networking::Request &t_req,
                           utils::StringVector t_commands) {
  try {
    t_req.m_command = commands::find.at(t_commands[COMMAND]);

  } catch (std::exception &err) {
    t_req.m_reply = networking::reply::r_502;
    t_req.m_valid = false;
    LOG_ERROR("Could not parse command.");
  };
}

void Parser::parse_argument(networking::Request &t_req,
                            utils::StringVector t_commands) {
  try {
    t_req.m_argument = t_commands.at(ARGUMENT);
    utils::Helpers::trim_string(t_req.m_argument);

  } catch (std::exception &err) {

    t_req.m_reply = networking::reply::r_501;
    t_req.m_valid = false;
    LOG_ERROR("Could not parse aargument.");
  }
}

void Parser::parse(networking::Request &t_req) {

  utils::StringVector split_command = utils::Helpers::split_string(t_req.m_raw);
  parse_command(t_req, split_command);

  commands::type type = commands::Utils::get_type(t_req.m_command);

  if (!t_req.m_valid || type == commands::type::no_argument)
    return;

  parse_argument(t_req, split_command);
}
