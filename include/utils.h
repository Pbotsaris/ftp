#ifndef UTILS_H
#define UTILS_H
#include "request.h"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>



namespace utils {
typedef std::vector<std::string> StringVector;

/* NOTE: Helper functions will always modify string in place */

struct Helpers {
  static StringVector split_string(std::string &s, std::string del = " ");
  static void trim_string(std::string &s);
  static void remove_last_path(std:: string &s);
  static void add_to_path(std::string &s, const std::string &s_to_add);

  private:
  static void trim_left(std::string &s);
  static void trim_right(std::string &s);
};

struct PathHelpers {

  static const std::string M_ROOT; 
  static const std::string M_SYS_PATH;

  static std::string build_system_path(networking::Request &t_req);
  static std::string join_to_user_path(networking::Request &t_req, std::string path_to_join);
  static std::string join_to_system_path(networking::Request &t_req, std::string path_to_join);
  static std::string create_system_root_path();


  /*  Conditional Checks */
  static bool path_exists(std::string &t_path);
  static bool is_working_dir_root(networking::Request &t_req);
  static bool is_absolute_path(std::string &t_path);

};

} // namespace utils

#endif // !UTILS_H UTILS_H
