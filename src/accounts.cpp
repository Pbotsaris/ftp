#include "../include/accounts.h"
#include "../include/doctest.h"

using namespace controllers;

std::string Accounts::ANONYMOUS_USER = "anonynous";
std::string Accounts::ANONYMOUS_PASSWORD = "0";

static const char PATH[] = ".users";
static const int USER = 0;
static const int PASSWORD = 1;

void Accounts::verify_user(networking::Request &t_req) {
  std::string pw;

  if (t_req.m_argument.empty()) {
    t_req.m_reply = networking::reply::r_501;
    return;
  }

  if (verify(t_req.m_argument, pw))
    t_req.m_reply = networking::reply::r_331;
  else
    t_req.m_reply = networking::reply::r_430; // invalid
}

void Accounts::verify_password(networking::Request &t_req) {

  if (t_req.m_argument.empty()) {
    t_req.m_reply = networking::reply::r_501;
    return;
  }

  if (verify(t_req.m_current_user, t_req.m_argument))
    t_req.m_reply = networking::reply::r_230;
  else
    t_req.m_reply = networking::reply::r_430;
}

bool Accounts::verify(std::string &t_user, std::string &t_password) {
  std::string user_acc;
  std::ifstream readfile(PATH);

  if (!readfile.good()) {
    create_file();
    readfile = std::ifstream(PATH);
  }

  while (readfile) {
    readfile >> user_acc;
    utils::StringVector user_acc_split =
        utils::StringHelpers::split_string(user_acc, ":");

    if (t_password.empty() ? has_user(user_acc_split, t_user)
                           : has_password(user_acc_split, t_user, t_password)) {
      readfile.close();
      return true;
    }
  }

  readfile.close();
  return false;
}

void Accounts::create_file() {
  std::ofstream outfile(PATH);

  outfile << ANONYMOUS_USER + ":" + ANONYMOUS_PASSWORD << std::endl;

  outfile.close();
}

bool Accounts::has_user(utils::StringVector &t_user_acc, std::string &t_user) {
  return t_user_acc[USER] == t_user;
}

bool Accounts::has_password(utils::StringVector &t_user_acc,
                            std::string &t_user, std::string &t_password) {

  return t_user_acc[USER] == t_user && t_user_acc[PASSWORD] == t_password;
}

TEST_CASE("User Controller") {

  SUBCASE("verify exiting user") {

    auto req = networking::Request();
    req.m_command = commands::USER;
    req.m_argument = "pedro";

    Accounts::verify_user(req);

    CHECK(req.m_reply == networking::reply::r_331);
  }

  SUBCASE("verify exiting password") {

    auto req = networking::Request();
    req.m_current_user = "pedro";
    req.m_command = commands::PASS;
    req.m_argument = "drops";

    Accounts::verify_password(req);

    CHECK(req.m_reply == networking::reply::r_230);
  }

 SUBCASE("verify bad password") {

    auto req = networking::Request();
    req.m_current_user = "pedro";
    req.m_command = commands::PASS;
    req.m_argument = "wrong";

    Accounts::verify_password(req);

    CHECK(req.m_reply == networking::reply::r_430);
  }


  SUBCASE("verify non-exiting user") {

    auto req = networking::Request();
    req.m_command = commands::USER;
    req.m_argument = "jonas";

    Accounts::verify_user(req);

    CHECK(req.m_reply == networking::reply::r_430);
  }

  SUBCASE("missing user / syntax error ") {

    auto req = networking::Request();
    req.m_command = commands::USER;
    req.m_argument = "";

    Accounts::verify_user(req);

    CHECK(req.m_reply == networking::reply::r_501);
  }

}
