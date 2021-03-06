#include "utils_string.hpp"
#include <algorithm>
#include <cstdlib>
#include<time.h>
#include<string.h>

using namespace utils;

StringVector StringHelpers::split_string(const std::string &s,
                                         const std::string delim) {
  int start = 0;
  int end = s.find(delim);
  StringVector result;

  while (end != -1) {
    result.push_back(s.substr(start, end - start));
    start = end + delim.size();
    end = s.find(delim, start);
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

std::string StringHelpers::join_string_vector(const StringVector &t_str_vec,
                                              const std::string &t_delim) {
  std::string result;

  for (auto &s : t_str_vec) {
    result.append(s + t_delim);
  }

  if (!t_delim.empty()) {
    result.pop_back();
  }

  return result;
}

std::string StringHelpers::random_string(const int length) {

  srand(time(0));
  std::string str(length, 0);
  std::generate_n(str.begin(), length, random_char);

  return str;
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

char StringHelpers::random_char(){
  const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    const size_t max_index = strlen(charset) - 1;
    return charset[rand() % max_index];
}
