#ifndef UTILS_FILE_H
#define UTILS_FILE_H
#include "request.hpp"
#include "connection.hpp"
#include <filesystem>
#include <memory>
#include <tuple>

#if __linux__ 
namespace fs = std::filesystem;
#elif __APPLE__ || __MACH__
namespace fs = std::__fs::filesystem;
#endif

namespace utils {


/* NOTE: FileHelpers is only concerned with Disk IO. Methods does not modify any of its arguments.*/

struct FileHelpers {
  
 enum listdir_option{list_name, list_stat};

  typedef  std::tuple<networking::ImageBuffer, std::uintmax_t>  DataFromDiskTuple;

  static std::string             list_dir_filenames(const networking::Request &t_req, listdir_option t_option);
  static std::string             stat_file(const networking::Request &t_req);
  static std::string             stat_file(const networking::Request &t_req, const std::string &t_path);
  static DataFromDiskTuple       read_bytes(const networking::Request &t_req);
  static void                    create_file(const networking::Request &t_req);
  static void                    write_to_disk(const networking::Request &t_req, const networking::DatafromClientTuple &t_data);

  private:
  static std::string             append_stat_result(const std::string t_path, const std::string t_filename, fs::file_status t_status);

  /* stat helpers */
  static fs::file_status         file_status_validate(const networking::Request &t_req, const std::string &t_path_to_stat);
  static std::string             file_permissions(const fs::perms t_perms);
  static std::string             file_type(const fs::file_status t_status);
  static std::string             file_size(const fs::file_status t_status, const std::string &t_path);
  static std::string             updated_at(const std::string &t_path);
  static std::string             file_group_user(const std::string &t_path);
  static std::string             list_dir_err_msg(const networking::Request &t_req);


  static void                    validate_path(const networking::Request &t_req, const std::string &t_path);

  template <typename TP>
  static std::time_t to_time_t(TP tp);
};

} // namespace utils

#endif // !UTILS_FILE_H
