#include "../../include/system_info.h"
#include "../../include/logger.hpp"
#include "../../include/doctest.h"
#include "../../include/disk_manager.h"
#include <algorithm>
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
    to_upper(t_req.m_argument);
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

    for (const auto & entry : fs::directory_iterator(t_req.m_argument)) {
        std::cout << entry.path() << std::endl;

    }
    
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

void SystemInfo::to_upper(std::string &t_str) {
  std::transform(t_str.begin(), t_str.end(), t_str.begin(), ::toupper);
}

//
//TEST_CASE("System Info") {
//
//   Disk disk;
//   DiskManager::init(disk);
//   auto req = netowrking::Request(disk);
//   req.m_argument = "/";
//
//   SysSystemInfo::status(req);
//
//    
//
//}
