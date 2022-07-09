#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H
#include <filesystem>
#include <fstream>

#include "logger.hpp"
#include "request.hpp"
#include "disk.hpp"
#include "disk_manager.hpp"

namespace controllers {

  struct SystemInfo {
    static void        system_os(networking::Request &t_req);
    static void        help(networking::Request &t_req);
    static void        status(networking::Request &t_req);
    static void        feature(networking::Request &t_req);

    private:
    static void        get_help(networking::Request &t_req);
    static void        list_help(networking::Request &t_req);
    static void        list_directory_items(networking::Request &t_req);
    static void        get_file_stat(networking::Request &t_req);

    static std::string get_os_name();
  };

}


#endif
