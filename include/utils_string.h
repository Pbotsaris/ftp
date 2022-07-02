#ifndef UTILS_STRING_H
#define UTILS_STRING_H
#include "request.h"
#include <string>
#include <vector>
#include <algorithm>

namespace utils {
typedef std::vector<std::string> StringVector;

struct StringHelpers {
  static StringVector split_string(std::string &s, std::string del = " ");
  static void trim_string(std::string &s);
  static void to_upper(std::string &s);

private:
  static void trim_left(std::string &s);
  static void trim_right(std::string &s);
};

} // namespace utils

#endif // !UTILS_STRING_H
