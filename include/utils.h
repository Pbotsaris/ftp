#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>

namespace utils {
typedef std::vector<std::string> StringVector;

struct Helpers {
  static StringVector split_string(std::string &s, std::string del = " ");
};

} // namespace utils

#endif // !UTILS_H UTILS_H
