#include "../../include/parser.h"
#include "../../include/doctest.h"
#include "../../include/disk.h"

#include <array>
#include <exception>

using namespace parsing;

static const int COMMAND = 0;
static const int ARGUMENT = 1;

void Parser::parse_command(networking::Request &t_req,
                           utils::StringVector t_commands) {
  try {
    utils::Helpers::trim_string(t_commands[COMMAND]);

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
    LOG_ERROR("Could not parse argument.");
  }
}

void Parser::parse(networking::Request &t_req) {


  utils::StringVector split_command = utils::Helpers::split_string(t_req.m_raw);
  parse_command(t_req, split_command);


  commands::type type = commands::Utils::get_type(t_req.m_command);


  if (!t_req.m_valid || type == commands::type::no_argument)
    return;

  if(commands::zero_or_more_arguments && split_command.size() < 2) /* if no argument is passed */
    return;


  parse_argument(t_req, split_command);
}

TEST_CASE("Parser raw command") {

  disk::Disk disk;
  disk.m_dir_level = 0;
  disk.m_system_path = "example";
  disk.m_user_path = "example";

  SUBCASE("With argument") {

    auto req = networking::Request(disk);
    req.m_raw = "USER marina";

    Parser::parse(req);

    CHECK(req.m_command == commands::USER);
    CHECK(req.m_argument == "marina");
  }

  SUBCASE("without argument") {

    auto req = networking::Request(disk);
    req.m_raw = "QUIT\n";

    Parser::parse(req);

    CHECK(req.m_command == commands::QUIT);
    CHECK(req.m_argument.empty());
  }

  SUBCASE("syntax error") {
    auto req = networking::Request(disk);
    req.m_raw = "WRONG";

    Parser::parse(req);

    CHECK(req.m_reply == networking::reply::r_502);
    CHECK(req.m_argument.empty());
  }
}
