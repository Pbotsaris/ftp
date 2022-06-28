#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>

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

} // namespace utils

#endif // !UTILS_H UTILS_H
