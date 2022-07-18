#include "utils_file.hpp"
#include "logger.hpp"
#include "utils_path.hpp"

#include <exception>
#include <fcntl.h>
#include <streambuf>
#include <unistd.h>

/* file Streams */
#include <fstream>
#include <ios>
#include <sstream>

/* updated at */
#include <chrono>
#include <filesystem>
#include <iomanip>

/* user & group */
#include <pwd.h>
#include <sys/stat.h>

#include <grp.h>
/* TEST HEADERS */
#include "disk.hpp"
#include "disk_manager.hpp"
#include "doctest.h"

using namespace utils;

/***** Public ******/

std::string FileHelpers::list_dir_filenames(const networking::Request &t_req, listdir_option t_option) {

  const std::string path_to_list = utils::PathHelpers::join_to_system_path(t_req, t_req.m_argument);

  std::string filenames;

  if (!utils::PathHelpers::path_exists(path_to_list)){
    throw list_dir_err_msg(t_req);
  }

  for (const auto &entry : fs::directory_iterator(path_to_list)) {
    if (t_option == list_name) {
      filenames.append(utils::PathHelpers::extract_last_path(entry.path()) +
                       " ");

    } else {
      filenames.append(stat_file(t_req, entry.path()));
      filenames.append("\r\n");
    }
  }

  return filenames;
}

/** **/

std::string FileHelpers::stat_file(const networking::Request &t_req) {

  std::string path_to_stat = PathHelpers::join_to_system_path(t_req, t_req.m_argument);
  fs::file_status status   = file_status_validate(t_req, path_to_stat);
  std::string filename     = utils::PathHelpers::extract_last_path(t_req.m_argument);

  return append_stat_result(path_to_stat, filename, status);
}


/** **/

std::string FileHelpers::stat_file(const networking::Request &t_req, const std::string &t_path) {

  fs::file_status status = file_status_validate(t_req, t_path);
  std::string filename   = utils::PathHelpers::extract_last_path(t_path);

  return append_stat_result(t_path, filename, status);
}

/** **/

FileHelpers::DataFromDiskTuple FileHelpers::read_bytes(const networking::Request &t_req) {

  std::string path_to_read = PathHelpers::join_to_system_path(t_req, t_req.m_argument);

  validate_path(t_req, path_to_read, false);

  std::ifstream file(path_to_read, std::ios::in | std::ios::binary | std::ios::ate);

  if (!file.is_open())
    throw "Could not open file for transfer";

  std::uintmax_t file_size = std::filesystem::file_size(path_to_read);
  networking::ImageBuffer buffer(new char[file_size]);
  file.read(buffer.get(), file_size);
  file.close();

  FileHelpers::DataFromDiskTuple alloced(std::move(buffer), file_size);

  return alloced;
}

/** **/

void FileHelpers::write_to_disk(const networking::Request &t_req, const networking::DatafromClientTuple &t_data) {

  fs::path path{t_req.m_disk.m_system_path + "/" + t_req.m_argument};

   /* overwrites existing files */
  if(PathHelpers::path_exists(path)){
    fs::remove(path);
  }

  std::ofstream file(path.c_str(), std::ios::out | std::ios::binary);
  file.write(std::get<0>(t_data).get(), std::get<1>(t_data));
  file.close();
}


/***** Private ******/

std::string FileHelpers::append_stat_result(const std::string t_path, const std::string t_filename,
                                            fs::file_status t_status) {

  std::string result = file_type(t_status);

  result.append(file_permissions(t_status.permissions()) + " ");
  result.append(std::to_string(fs::hard_link_count(t_path)) + " ");
  result.append(file_group_user(t_path) + " ");
  result.append(file_size(t_status, t_path) + " ");
  result.append(updated_at(t_path) + " \t");
  result.append(t_filename);

  return result;
}

fs::file_status FileHelpers::file_status_validate(const networking::Request &t_req,
                                  const std::string &t_path_to_stat) {

  validate_path(t_req, t_path_to_stat);

  std::error_code err;
  err.clear();

  fs::file_status status = fs::status(t_path_to_stat, err);

  if (err.value()) {
    std::string user_path =
        PathHelpers::join_to_user_path(t_req, t_req.m_argument);
    throw "Could not stat " + user_path;
  }

  return status;
}

void FileHelpers::validate_path(const networking::Request &t_req, const std::string &t_path, const bool t_allow_dir) {

 std::string user_path = PathHelpers::join_to_user_path(t_req, t_req.m_argument);

  if (!PathHelpers::path_exists(t_path)) {
    throw " The path \'" + user_path + "\' does not exist.";
  }

  if (!t_allow_dir && PathHelpers::is_path_directory(t_path)) {
    throw " The path \'" + user_path + "\' is a directory.";
  }
}

std::string FileHelpers::file_type(const fs::file_status t_status) {
  if (fs::is_regular_file(t_status))
    return "-";
  if (fs::is_directory(t_status))
    return "d";
  if (fs::is_block_file(t_status))
    return "b";
  if (fs::is_character_file(t_status))
    return "c";
  if (fs::is_fifo(t_status))
    return "p";
  if (fs::is_socket(t_status))
    return "s";
  if (fs::is_symlink(t_status))
    return "l";
  else
    return "Unkown";
}

std::string FileHelpers::file_size(const fs::file_status t_status, const std::string &t_path){

   if (fs::is_directory(t_status))
       return "4096"; // dir always 4 bytes
   else
       return std::to_string(fs::file_size(t_path));
}

std::string FileHelpers::file_permissions(const fs::perms t_perms) {
  std::stringstream permissions;
  permissions
      << ((t_perms & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
      << ((t_perms & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
      << ((t_perms & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
      << ((t_perms & fs::perms::group_read) != fs::perms::none ? "r" : "-")
      << ((t_perms & fs::perms::group_write) != fs::perms::none ? "w" : "-")
      << ((t_perms & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
      << ((t_perms & fs::perms::others_read) != fs::perms::none ? "r" : "-")
      << ((t_perms & fs::perms::others_write) != fs::perms::none ? "w" : "-")
      << ((t_perms & fs::perms::others_exec) != fs::perms::none ? "x" : "-");

  return permissions.str();
}

std::string FileHelpers::updated_at(const std::string &t_path) {

  std::stringstream time;
  fs::file_time_type ftime = std::filesystem::last_write_time(t_path);
  std::time_t cftime = to_time_t(ftime);

  time << std::asctime(std::localtime(&cftime));

  std::string time_str = time.str();
  time_str = time_str.substr(0, time_str.find_last_of("\\:")); // rm Week day, seconds and year
  time_str = time_str.substr(4, time_str.size()); // rm Week day, seconds and year
                                                             
  return time_str;

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

std::string FileHelpers::list_dir_err_msg(const networking::Request &t_req) {

  return "Failed listing directory; The path '" +
         utils::PathHelpers::join_to_user_path(t_req, t_req.m_argument) +
         "' does not exist.";
}

template <typename TP> std::time_t FileHelpers::to_time_t(TP tp) {
  using namespace std::chrono;
  auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() +
                                                      system_clock::now());
  return system_clock::to_time_t(sctp);
}

/* TESTS */

TEST_CASE("File Helpers") { // create file to test

  SUBCASE("stat a file") {
    std::string path = ".root/test.txt";
    auto a_file = std::ofstream(path);

    disk::Disk disk;
    controllers::DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "test.txt";

    auto s_stat = FileHelpers::stat_file(req);
    CHECK(!s_stat.empty());

    fs::remove(path);
  }

  SUBCASE("Read file from the disk") {

    disk::Disk disk;
    controllers::DiskManager::init(disk);
    auto req = networking::Request(disk);

    req.m_argument = "image.png";
  }
}
