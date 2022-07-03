#include "utils_file.hpp"
#include "logger.hpp"
#include "utils_path.hpp"

/* updated at */
#include <chrono>
#include <iomanip>

/* user & group */
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>

/* TEST HEADERS */
#include "disk.hpp"
#include "disk_manager.hpp"
#include "doctest.h"

using namespace utils;

std::string FileHelpers::list_dir_filenames(const networking::Request &t_req) {

  const std::string path_to_list =
      utils::PathHelpers::join_to_system_path(t_req, t_req.m_argument);
  std::string filenames;

  if (!utils::PathHelpers::path_exists(path_to_list))
    throw "Failed listing directory; The path '" +
        utils::PathHelpers::join_to_user_path(t_req, t_req.m_argument) +
        "' does not exist.";

  for (const auto &entry : fs::directory_iterator(path_to_list)) {
    filenames.append(utils::PathHelpers::extract_last_path(entry.path()) + " ");
  }

  return filenames;
}

std::string FileHelpers::stat_file(const networking::Request &t_req) {

  std::string path_to_stat = PathHelpers::join_to_system_path(t_req, t_req.m_argument);
  fs::file_status status   = file_status_validate(t_req, path_to_stat);
  std::string result       = file_permissions(status.permissions()) + " ";

  result.append(file_group_user(path_to_stat) + " ");
  result.append(updated_at(path_to_stat) + " ");
  result.append(utils::PathHelpers::extract_last_path(t_req.m_argument) + " (");
  result.append(file_type(status) + ")");

  return result;
}

/* PRIVATE */

fs::file_status FileHelpers::file_status_validate(const networking::Request &t_req, const std::string &t_path_to_stat){

  std::string err_path = PathHelpers::join_to_user_path(t_req, t_req.m_argument);

  if(!PathHelpers::path_exists(t_path_to_stat))
     throw "Failed to stat; The path " + err_path + "  does not exist.";

  std::error_code err;
  err.clear();

  fs::file_status status = fs::status(t_path_to_stat, err);

  if(err.value())
      throw "Could not stat " + err_path;

 return status;
}

std::string FileHelpers::file_type(const fs::file_status t_status) {
  if (fs::is_regular_file(t_status))
    return "Regular file";
  if (fs::is_block_file(t_status))
    return "Block";
  if (fs::is_character_file(t_status))
    return "Char";
  if (fs::is_fifo(t_status))
    return "FIFO";
  if (fs::is_socket(t_status))
    return "Cocket";
  if (fs::is_symlink(t_status))
    return "Symlink";
  else
    return "Unkown";
}
std::string FileHelpers::file_permissions(const fs::perms t_perms) {
  std::stringstream permissions;
  permissions
      << ((t_perms & fs::perms::owner_read)   != fs::perms::none ? "r" : "-")
      << ((t_perms & fs::perms::owner_write)  != fs::perms::none ? "w" : "-")
      << ((t_perms & fs::perms::owner_exec)   != fs::perms::none ? "x" : "-")
      << ((t_perms & fs::perms::group_read)   != fs::perms::none ? "r" : "-")
      << ((t_perms & fs::perms::group_write)  != fs::perms::none ? "w" : "-")
      << ((t_perms & fs::perms::group_exec)   != fs::perms::none ? "x" : "-")
      << ((t_perms & fs::perms::others_read)  != fs::perms::none ? "r" : "-")
      << ((t_perms & fs::perms::others_write) != fs::perms::none ? "w" : "-")
      << ((t_perms & fs::perms::others_exec)  != fs::perms::none ? "x" : "-");

  return permissions.str();
}

std::string FileHelpers::updated_at(const std::string &t_path) {

  std::stringstream time;
  fs::file_time_type ftime = std::filesystem::last_write_time(t_path);
  std::time_t cftime = to_time_t(ftime);

  time << std::asctime(std::localtime(&cftime));

  return time.str();
}
std::string FileHelpers::file_group_user(const std::string &t_path) {

  struct stat file_info;

  if ((stat(t_path.c_str(), &file_info)) < 0)
    throw "Could not get stat user and group from " + t_path + ".";

  struct passwd *pw = getpwuid(file_info.st_uid);
  struct group *gr = getgrgid(file_info.st_gid);

  if (pw == 0 || gr == 0)
    throw "Could not get stat user and group from " + t_path + ".";

  return std::string(pw->pw_name) + " " + std::string(gr->gr_name);
}


template <typename TP>
std::time_t FileHelpers::to_time_t(TP tp) {
  using namespace std::chrono;
  auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() +
                                                      system_clock::now());
  return system_clock::to_time_t(sctp);
}

/* TESTS */

TEST_CASE("File Utils") { // create file to test
  std::string path = ".root/test.txt";
  auto a_file = std::ofstream(path);

  disk::Disk disk;
  controllers::DiskManager::init(disk);
  auto req = networking::Request(disk);
  req.m_argument = "test.txt";

  auto stat = FileHelpers::stat_file(req);
  CHECK(!stat.empty());

  fs::remove(path);
}
