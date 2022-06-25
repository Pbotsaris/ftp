#include "../../include/users.h"
#include <iostream>

using namespace accounts;

static const char PATH[] = ".users";
static const int USER = 0;
static const int PASSWORD = 1;

bool Users::verify_user(std::string &t_user) {
  std::string pw = "";
  return verify(t_user, pw);
}

bool Users::verify_password(std::string &t_user, std::string &t_password) {

  return verify(t_user, t_password);
}

bool Users::verify(std::string &t_user, std::string &t_password) {
  std::string user_acc;
  std::ifstream readfile(PATH);

  if (!readfile.good()) {
    create_file();
    readfile = std::ifstream(PATH);
  }

  while (readfile) {
    readfile >> user_acc;
    utils::StringVector user_acc_split =
        utils::Helpers::split_string(user_acc, ":");

    if (t_password.empty() ? has_user(user_acc_split, t_user) 
       : has_password(user_acc_split, t_user, t_password)) {
      readfile.close();
      return true;
    }
  }

  readfile.close();
  return false;
}

void Users::create_file() {
  std::ofstream outfile(PATH);

  outfile << "Anonynous:0" << std::endl;

  outfile.close();
}

bool Users::has_user(utils::StringVector &t_user_acc, std::string &t_user){
  return t_user_acc[USER] == t_user;
}

bool Users::has_password(utils::StringVector &t_user_acc, std::string &t_user, std::string &t_password){
  return t_user_acc[USER] == t_user && t_user_acc[PASSWORD] == t_password;
}
