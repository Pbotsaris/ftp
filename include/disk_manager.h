#ifndef DISK_MANAGER_H
#define DISK_MANAGER_H
#include "disk.h"
#include "request.h"
#include "utils.h"
#include "logger.hpp"
#include "reply.hpp"
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <iostream>

namespace controllers {
struct DiskManager {
  const static std::string M_ROOT;
  const static std::string M_SYS_PATH;

  static void init(disk::Disk &t_disk);
  /* Callbacks */
  static void change_directory(networking::Request &t_req);
  static void change_up_directory(networking::Request &t_req);
  static void print_working_directory(networking::Request &t_req);

  private:
  /* CWD */
  static void change_bad_path(networking::Request &t_req);
  static void change_valid_path(networking::Request &t_req);
  static void change_absolute_path(networking::Request &t_req);

  static void update_paths(networking::Request &t_req, const utils::StringVector &t_paths);
  static bool path_exists(std::string &t_path);
  static bool is_absolute_path(std::string &t_path);

  static void remove_trailing_slash(networking::Request &t_req);
  static int count_resuling_dir_level(utils::StringVector &t_paths);

};

} // namespace disk


#endif // !DISK_MANAGER_H
