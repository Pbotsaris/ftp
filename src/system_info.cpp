#include "../include/system_info.h"
#include "../include/doctest.h"
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

void SystemInfo::get_help(networking::Request &t_req) {
  try {
    t_req.m_reply_msg = commands::help.at(t_req.m_argument);
  } catch (std::exception &_err) {
    t_req.m_reply = networking::reply::r_500;
    return;
  }

  t_req.m_reply = networking::reply::r_214;
}

void SystemInfo::status(networking::Request &t_req) {

  if (t_req.m_argument.empty()) {
    // TODO: Check if during file transfer.
    // return information about the transfer.
    // returns an error if a file is not transfering
  }
  // return information about a path
  else {
    list_directory_items(t_req);
  }
}

/* PRIVATE */

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
    t_req.m_reply_msg = utils::FileHelpers::list_dir_filenames(t_req);
    t_req.m_reply = networking::reply::r_212;

  } catch (std::string &err) {
    t_req.m_reply = networking::reply::r_550;
    LOG_ERROR(err.c_str());
  }
}

std::string SystemInfo::get_os_name() {
#ifdef _WIN32
  return " Windows 32-bit";
#elif _WIN64
  return " Windows 64-bit";
#elif __APPLE__ || __MACH__
  return " Mac OSX";
#elif __linux__
  return " Linux";
#elif __FreeBSD__
  return " FreeBSD";
#elif __unix || __unix__
  return " Unix";
#else
  return " Other";
#endif
}

TEST_CASE("System Info") {

  fs::create_directories(utils::PathHelpers::M_ROOT + "/first_dir");
  fs::create_directories(utils::PathHelpers::M_ROOT + "/second_dir");
  fs::create_directories(utils::PathHelpers::M_ROOT + "/third_dir");

  SUBCASE("List existing directory names") {

    disk::Disk disk;
    DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "/";

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

  fs::remove_all(utils::PathHelpers::M_ROOT + "/first_dir");
  fs::remove_all(utils::PathHelpers::M_ROOT + "/second_dir");
  fs::remove_all(utils::PathHelpers::M_ROOT + "/third_dir");
}
