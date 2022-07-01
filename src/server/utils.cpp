#include "../../include/utils.h"
#include "../../include/doctest.h"

#include <algorithm>

#if __unix || __unix__
namespace fs = std::filesystem;

#elif __APPLE__ || __MACH__
namespace fs = std::__fs::filesystem;

#endif


using namespace utils;

const std::string PathHelpers::M_ROOT = ".root";
const std::string PathHelpers::M_SYS_PATH = fs::current_path();


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


/* PATH HELPERS */

bool PathHelpers::path_exists(std::string &t_path) {
  const fs::path p(t_path);
  return fs::exists(p);
}

bool PathHelpers::is_absolute_path(std::string &t_path) {
  return t_path[0] == '/';
}

bool PathHelpers::is_working_dir_root(networking::Request &t_req) {
  return t_req.m_disk.m_user_path == "/";
}

std::string PathHelpers::build_system_path(networking::Request &t_req) {
  return is_absolute_path(t_req.m_argument)
             ? create_system_root_path() + "/" + t_req.m_argument
             : join_to_system_path(t_req, t_req.m_argument);
}

std::string PathHelpers::join_to_user_path(networking::Request &t_req,
                                           std::string path_to_join) {
  return is_working_dir_root(t_req)
             ? t_req.m_disk.m_user_path + path_to_join
             : t_req.m_disk.m_user_path + "/" + path_to_join;
}

std::string PathHelpers::join_to_system_path(networking::Request &t_req,
                                             std::string path_to_join) {
  return t_req.m_disk.m_system_path + "/" + path_to_join;
}

std::string PathHelpers::create_system_root_path() {

  return M_SYS_PATH + "/" + M_ROOT;
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
