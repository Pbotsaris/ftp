#ifndef UTILS_H
#define UTILS_H
#include "request.h"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

namespace utils {
typedef std::vector<std::string> StringVector;

/* NOTE: StringHelper functions will modify string in place */

struct StringHelpers {
  static StringVector      split_string(std::string &s, std::string del = " ");
  static void              trim_string(std::string &s);

  private:
  static void              trim_left(std::string &s);
  static void              trim_right(std::string &s);
};


/* NOTE: PathHelper never modifies arguments and will always return a new string. */

struct PathHelpers {

  static const std::string M_ROOT; 
  static const std::string M_SYS_PATH;

  static std::string       build_system_path(const networking::Request &t_req);
  static std::string       join_to_user_path(const networking::Request &t_req, const std::string path_to_join);
  static std::string       join_to_system_path (const networking::Request &t_req, const std::string path_to_join);
  static std::string       create_system_root_path();
  static std::string       add_to_path(const std::string &t_path, const std::string &t_path_to_add);
  static std::string       remove_last_path(const std:: string &t_path);


  /*  Conditional Checks */
  static bool              path_exists(const std::string &t_path);
  static bool              is_working_dir_root(const networking::Request &t_req);
  static bool              is_absolute_path(const std::string &t_path);
};

} // namespace utils

#endif // !UTILS_H UTILS_H
