#include "utils_path.hpp"
#include <filesystem>
#include <system_error>

#if __unix || __unix__
namespace fs = std::filesystem;

#elif __APPLE__ || __MACH__
namespace fs = std::__fs::filesystem;

#endif

using namespace utils;

const std::string PathHelpers::M_ROOT = ".root";
const std::string PathHelpers::M_SYS_PATH = fs::current_path();

bool PathHelpers::path_exists(const std::string &t_path) {
  const fs::path p(t_path);
  return fs::exists(p);
}

bool PathHelpers::is_absolute_path(const std::string &t_path) {
  return t_path[0] == '/';
}

bool PathHelpers::is_working_dir_root(const networking::Request &t_req) {
  return t_req.m_disk.m_user_path == "/";
}

bool PathHelpers::is_path_directory(const std::string &t_path) {
  std::error_code err;
  err.clear();
  fs::file_status status = fs::status(t_path, err);

  if (err.value()) {
    return false;
  }

  return fs::is_directory(status);
}

std::string PathHelpers::build_system_path(const networking::Request &t_req) {
  return is_absolute_path(t_req.m_argument)
             ? create_system_root_path() + "/" + t_req.m_argument
             : join_to_system_path(t_req, t_req.m_argument);
}

std::string PathHelpers::join_to_user_path(const networking::Request &t_req,
                                           const std::string path_to_join) {
  return is_working_dir_root(t_req)
             ? t_req.m_disk.m_user_path + path_to_join
             : t_req.m_disk.m_user_path + "/" + path_to_join;
}

std::string PathHelpers::join_to_system_path(const networking::Request &t_req,
                                             const std::string path_to_join) {
  return t_req.m_argument == "/"
             ? t_req.m_disk.m_system_path
             : t_req.m_disk.m_system_path + "/" + path_to_join;
}

std::string PathHelpers::create_system_root_path() {

  return M_SYS_PATH + "/" + M_ROOT;
}

std::string PathHelpers::add_to_path(const std::string &t_path,
                                     const std::string &t_path_to_join) {

  std::string result;

  if (t_path == "/")
    result = t_path + t_path_to_join;
  else
    result = t_path + "/" + t_path_to_join;

  return result;
}

std::string PathHelpers::remove_last_path(const std::string &t_path) {
  std::string result = t_path.substr(0, t_path.find_last_of("\\/"));

  if (result.empty())
    result = "/";

  return result;
}

std::string PathHelpers::extract_last_path(const std::string &t_path) {
  return t_path.substr(t_path.find_last_of("\\/") + 1, t_path.size());
};
