#ifndef UTILS_STRING_H
#define UTILS_STRING_H
#include "request.hpp"
#include <string>
#include <vector>

namespace utils {
typedef std::vector<std::string> StringVector;

struct StringHelpers {
  static StringVector split_string(const std::string &s, const std::string delim = " ");
  static void trim_string(std::string &s);
  static void to_upper(std::string &s);
  static std::string join_string_vector(const StringVector &t_str_vec, const std::string &t_delim = "");

private:
  static void trim_left(std::string &s);
  static void trim_right(std::string &s);
};

} // namespace utils

#endif // !UTILS_STRING_H
