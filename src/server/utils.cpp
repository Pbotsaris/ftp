#include "../../include/utils.h"
#include "../../include/doctest.h"

using namespace utils;

StringVector Helpers::split_string(std::string &s, std::string del) {
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

void Helpers::trim_string(std::string &s){
  trim_left(s);
  trim_right(s);
}

void Helpers::remove_last_path(std:: string &s){
   s = s.substr(0, s.find_last_of("\\/"));

   if(s.empty()) 
     s = "/";
}

void Helpers::add_to_path(std::string &s, const std::string &s_to_add){
  if(s == "/")
    s = s + s_to_add;
  else
   s = s + "/" + s_to_add;
}

/* PRIVATE */

void Helpers::trim_left(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return (!std::isspace(ch)) && ch != '\n' && ch != '\t' &&
                   ch != '\r';
          }));
}

void Helpers::trim_right(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) {
                         return (!std::isspace(ch)) && ch != '\n' &&
                                ch != '\t' && ch != '\r';
                       })
              .base(),
          s.end());
}

TEST_CASE("Helpers") {

  SUBCASE("remove path") {
  std::string s = "/some/path/example";
  Helpers::remove_last_path(s);
  CHECK(s == "/some/path");
  }

  SUBCASE("add path") {
    std::string s = "/some/path";
    std::string s_to_add = "example";

    Helpers::add_to_path(s, s_to_add);

    CHECK(s == "/some/path/example");

  }

 SUBCASE("add path when root") {
    std::string s = "/";
    std::string s_to_add = "example";

    Helpers::add_to_path(s, s_to_add);

    CHECK(s == "/example");

  }


}
