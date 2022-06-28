
#ifndef DISK_H
#define DISK_H
#include <string>

namespace disk {
struct Disk {
  std::string m_user_path;
  std::string m_system_path;
  int         m_dir_level = 0;
};
} // namespace disk

#endif
