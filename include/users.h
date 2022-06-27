#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "utils.h"
#include "request.h"
#include "reply.hpp"
#include "logger.hpp"
#include <fstream>
#include <string>

namespace controllers {

struct Accounts {
  static std::string ANONYMOUS_USER;
  static std::string ANONYMOUS_PASSWORD;

  static void verify_user(networking::Request &t_req);
  static void verify_password(networking::Request &t_req);

private:
  static bool verify(std::string &t_user, std::string &password);
  static bool has_user(utils::StringVector &t_user_acc, std::string &t_user);
  static bool has_password(utils::StringVector &t_user_acc, std::string &t_user, std::string &t_password);
  static void create_file();
};


} // namespace accounts
  


#endif
