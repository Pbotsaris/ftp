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
  static void change_up_directory(networking::Request &t_req);
  static void print_working_directory(networking::Request &t_req);
  static void make_directory(networking::Request &t_req);
  static void remove_directory(networking::Request &t_req);
  static void change_directory(networking::Request &t_req);

  private:
  /* CWD Helpers */
  static void change_bad_path(networking::Request &t_req);
  static void change_valid_path(networking::Request &t_req);
  static void change_absolute_path(networking::Request &t_req);
  static void update_paths(networking::Request &t_req, const utils::StringVector &t_paths);

  /* RMD Helpers */
  static void remove_directory_when_valid(networking::Request &t_req, std::string &t_to_delete);

  /*  Conditional Checks */
  static bool path_exists(std::string &t_path);
  static bool is_absolute_path(std::string &t_path);
  static bool is_working_dir_root(networking::Request &t_req);

  /* Path Manipulation */
  static std::string join_to_user_path(networking::Request &t_req, std::string path_to_join);
  static std::string join_to_system_path(networking::Request &t_req, std::string path_to_join);
  static std::string create_system_root_path();

  /* Misc */
  static void remove_trailing_slash(networking::Request &t_req);
  static int count_resuling_dir_level(utils::StringVector &t_paths);
  static int count_directory_items(std::string &t_path);



};

} // namespace disk


#endif // !DISK_MANAGER_H
