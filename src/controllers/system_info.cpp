#include "system_info.hpp"
#include "doctest.h"
#include "utils_file.hpp"
#include "utils_path.hpp"
#include <exception>

using namespace controllers;

#if __linux__
namespace fs = std::filesystem;

#elif __APPLE__ || __MACH__
namespace fs = std::__fs::filesystem;

#endif

void SystemInfo::system_os(networking::Request &t_req) {

  t_req.m_reply = networking::reply::r_215;
  t_req.m_reply_msg = get_os_name();
}

void SystemInfo::help(networking::Request &t_req) {

  if (t_req.m_argument.empty()) {
    list_help(t_req);
  } else {
    utils::StringHelpers::to_upper(t_req.m_argument);
    get_help(t_req);
  }
}

void SystemInfo::status(networking::Request &t_req) {

  if (t_req.m_argument.empty()) {
    t_req.m_reply = networking::reply::r_502; // info on the transfer not implemented by this server.
    return;
  }

  try {

  std::string path = utils::PathHelpers::join_to_system_path(t_req, t_req.m_argument);


    if (utils::PathHelpers::is_path_directory(path))
      list_directory_items(t_req);
    else
      get_file_stat(t_req);
  } catch (std::string &err) {

    LOG_ERROR(err.c_str());

    t_req.m_reply = networking::reply::r_550;
  }
}

void SystemInfo::feature(networking::Request &t_req) {

  std::string features;
  for (std::map<std::string, std::string>::iterator iter =
           commands::help.begin();
       iter != commands::help.end(); ++iter) {

    features = features + iter->first + " ";
  }

  t_req.m_reply = networking::reply::r_211;
  t_req.m_reply_msg = features + "\n";
}

/* PRIVATE */

void SystemInfo::get_help(networking::Request &t_req) {
  try {
    t_req.m_reply_msg = commands::help.at(t_req.m_argument);
  } catch (std::exception &_err) {
    t_req.m_reply = networking::reply::r_500;
    return;
  }

  t_req.m_reply = networking::reply::r_214;
}

void SystemInfo::list_help(networking::Request &t_req) {

  int count = 0;
  t_req.m_reply_msg = "Available Commands:";

  for (auto const &help_command : commands::help) {

    std::string sep = count != 0 ? ", " : " ";
    t_req.m_reply_msg.append(sep + help_command.first);

    count++;
  }

  t_req.m_reply_msg.append(".");
  t_req.m_reply = networking::reply::r_214;
}

void SystemInfo::list_directory_items(networking::Request &t_req) {

  try {
    t_req.m_reply_msg = utils::FileHelpers::list_dir_filenames(
        t_req, utils::FileHelpers::list_name);
    t_req.m_reply = networking::reply::r_212;

  } catch (std::string &err) {
    t_req.m_reply = networking::reply::r_550;
    LOG_ERROR(err.c_str());
  }
}

void SystemInfo::get_file_stat(networking::Request &t_req) {

  try {
    t_req.m_reply_msg = utils::FileHelpers::stat_file(t_req);
    t_req.m_reply = networking::reply::r_214;

  } catch (std::string &err) {
    t_req.m_reply = networking::reply::r_550;
    LOG_ERROR(err.c_str());
  }
}

/* PRIVATE */

std::string SystemInfo::get_os_name() {
#ifdef _WIN32
  return "Windows 32-bit";
#elif _WIN64
  return "Windows 64-bit";
#elif __APPLE__ || __MACH__
  return "OSX";
#elif __linux__
  return "LINUX Type:L: 8";
#elif __FreeBSD__
  return "FreeBSD";
#elif __unix || __unix__
  return "UNIX Type: L8";
#else
  return "Other";
#endif
}

TEST_CASE("System Info") {

  fs::create_directories(utils::PathHelpers::M_ROOT + "/test");
  fs::create_directories(utils::PathHelpers::M_ROOT + "/test/first_dir");
  fs::create_directories(utils::PathHelpers::M_ROOT + "/test/second_dir");
  fs::create_directories(utils::PathHelpers::M_ROOT + "/test/third_dir");

  SUBCASE("List existing directory names") {

    disk::Disk disk;
    DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "/test";

    SystemInfo::status(req);

    CHECK(req.m_reply == networking::reply::r_212);
    CHECK(req.m_reply_msg == "first_dir second_dir third_dir ");
  }

  SUBCASE("List unexisting directory names") {

    disk::Disk disk;
    DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "bad/path";

    SystemInfo::status(req);

    CHECK(req.m_reply == networking::reply::r_550);
  }

  fs::remove_all(utils::PathHelpers::M_ROOT + "/test/first_dir");
  fs::remove_all(utils::PathHelpers::M_ROOT + "/test/second_dir");
  fs::remove_all(utils::PathHelpers::M_ROOT + "/test/third_dir");
  fs::remove_all(utils::PathHelpers::M_ROOT + "/test");
}
