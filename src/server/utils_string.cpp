#include "../../include/utils_string.h"

using namespace utils;

StringVector StringHelpers::split_string(std::string &s, std::string del) {
  int start = 0;
  int end = s.find(del);
  StringVector result;

  while (end != -1) {
    result.push_back(s.substr(start, end - start));
    start = end + del.size();
    end = s.find(del, start);
  }
  result.push_back(s.substr(start, end - start));

  return result;
}

void StringHelpers::trim_string(std::string &s) {
  trim_left(s);
  trim_right(s);
}

void StringHelpers::to_upper(std::string &t_str) {
  std::transform(t_str.begin(), t_str.end(), t_str.begin(), ::toupper);
}

/* PRIVATE */

void StringHelpers::trim_left(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return (!std::isspace(ch)) && ch != '\n' && ch != '\t' &&
                   ch != '\r';
          }));
}

void StringHelpers::trim_right(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) {
                         return (!std::isspace(ch)) && ch != '\n' &&
                                ch != '\t' && ch != '\r';
                       })
              .base(),
          s.end());
}

