#ifndef DISK_MANAGER_H
#define DISK_MANAGER_H
#include "request.hpp"
#include "utils.hpp"

namespace controllers {
struct DiskManager {
  static std::string m_rename_from;

  /* Disk initializer */
  static void        init(disk::Disk &t_disk);

  /* Callbacks */
  static void        rename_from(networking::Request &t_req);
  static void        rename_to(networking::Request &t_req);
  static void        change_up_directory(networking::Request &t_req);
  static void        print_working_directory(networking::Request &t_req);
  static void        make_directory(networking::Request &t_req);
  static void        remove_directory(networking::Request &t_req);
  static void        change_directory(networking::Request &t_req);

  private:
  /* CWD Helpers */
  static void        change_bad_path(networking::Request &t_req);
  static void        change_valid_path(networking::Request &t_req);
  static void        change_absolute_path(networking::Request &t_req);
  static void        update_paths(networking::Request &t_req, const utils::StringVector &t_paths);

  /* RMD Helpers */
  static void       remove_directory_when_valid(networking::Request &t_req, std::string &t_to_delete);

  /* Misc */
  static void       remove_trailing_slash(networking::Request &t_req);
  static int        count_resuling_dir_level(utils::StringVector &t_paths);
  static int        count_directory_items(std::string &t_path);


};

} // namespace disk


#endif // !DISK_MANAGER_H
