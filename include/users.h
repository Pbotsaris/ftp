#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "utils.h"
#include <fstream>
#include <string>

namespace accounts {

struct Users {
  static bool verify_user(std::string &t_user);
  static bool verify_password(std::string &t_user, std::string &t_password);

private:
  static bool verify(std::string &t_user, std::string &password);
  static bool has_user(utils::StringVector &t_user_acc, std::string &t_user);
  static bool has_password(utils::StringVector &t_user_acc, std::string &t_user, std::string &t_password);
  static void create_file();
};

} // namespace accounts

#endif
