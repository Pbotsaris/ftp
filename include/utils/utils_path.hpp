#ifndef UTILS_PATH_H
#define UTILS_PATH_H
#include "request.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
//#include <vector>

/* NOTE: PathHelper never modifies arguments and will always return a new
 * string. */

namespace utils {
struct PathHelpers {

  static const std::string M_ROOT;
  static const std::string M_SYS_PATH;

  static std::string build_system_path(const networking::Request &t_req);
  static std::string join_to_user_path(const networking::Request &t_req, const std::string path_to_join);
  static std::string join_to_system_path(const networking::Request &t_req, const std::string path_to_join);
  static std::string create_system_root_path();
  static std::string add_to_path(const std::string &t_path, const std::string &t_path_to_add);
  static std::string remove_last_path(const std::string &t_path);
  static std::string extract_last_path(const std::string &t_path);

  /*  Conditional Checks */
  static bool path_exists(const std::string &t_path);
  static bool is_working_dir_root(const networking::Request &t_req);
  static bool is_absolute_path(const std::string &t_path);
  static bool is_path_directory(const std::string &t_path);
};
} // namespace utils

#endif // !UTILS_PATH_H
       //
