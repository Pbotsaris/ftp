#include "../../include/disk_manager.h"
#include "../../include/doctest.h"
#include <exception>
#include <string>

using namespace controllers;
using namespace disk;
namespace fs = std::__fs::filesystem;

const std::string DiskManager::M_ROOT = ".root";
const std::string DiskManager::M_SYS_PATH = fs::current_path();

void DiskManager::init(Disk &t_disk) {
  t_disk.m_user_path = "/";
  t_disk.m_system_path = create_system_root_path();
  t_disk.m_dir_level = 0;

  fs::create_directories(M_ROOT);
};

void DiskManager::change_up_directory(networking::Request &t_req) {
  t_req.m_argument = "..";
  change_directory(t_req);
}

void DiskManager::print_working_directory(networking::Request &t_req) {

  t_req.m_reply = networking::reply::r_257;
  t_req.m_reply_msg = t_req.m_disk.m_user_path;
}
void DiskManager::make_directory(networking::Request &t_req) {

  if (is_absolute_path(t_req.m_argument)) {
    fs::create_directories(create_system_root_path() + t_req.m_argument);
    t_req.m_reply_msg = t_req.m_argument + " created.";

  } else {
    fs::create_directories(join_to_system_path(t_req, t_req.m_argument));
    t_req.m_reply_msg =
        join_to_user_path(t_req, t_req.m_argument) + " created.";
  }

  t_req.m_reply = networking::reply::r_257;
}

void DiskManager::remove_directory(networking::Request &t_req) {

  auto to_delete = is_absolute_path(t_req.m_argument)
                       ? create_system_root_path() + "/" + t_req.m_argument
                       : join_to_system_path(t_req, t_req.m_argument);

  LOG_DEBUG(to_delete.c_str());

  if (path_exists(to_delete)) {
    remove_directory_when_valid(t_req, to_delete);
  } else {
    t_req.m_reply = networking::reply::r_550;
  }
}

void DiskManager::change_directory(networking::Request &t_req) {

  if (is_absolute_path(t_req.m_argument)) {
    change_absolute_path(t_req);
    remove_trailing_slash(t_req);
    return;
  }

  utils::StringVector paths =
      utils::Helpers::split_string(t_req.m_argument, "/");

  int resulting_dir_level = count_resuling_dir_level(paths);

  if (t_req.m_disk.m_dir_level + resulting_dir_level < 0) {
    change_bad_path(t_req);
    return;
  }

  update_paths(t_req, paths);
  remove_trailing_slash(t_req);
  t_req.m_disk.m_dir_level = t_req.m_disk.m_dir_level + resulting_dir_level;
}

/* PRIVATE */

void DiskManager::update_paths(networking::Request &t_req, const utils::StringVector &t_paths) {

  std::string user_path = t_req.m_disk.m_user_path;
  std::string system_path = t_req.m_disk.m_system_path;

  for (auto &path : t_paths) {

    if (path == "..") {
      utils::Helpers::remove_last_path(user_path);
      utils::Helpers::remove_last_path(system_path);
    } else if (path == ".") {
      continue;
    }

    else {
      utils::Helpers::add_to_path(user_path, path);
      utils::Helpers::add_to_path(system_path, path);
    }
  }

  if (path_exists(system_path)) {
    t_req.m_disk.m_user_path = user_path;
    t_req.m_disk.m_system_path = system_path;
    change_valid_path(t_req);
  } else {

    change_bad_path(t_req);
  }
}

void DiskManager::remove_directory_when_valid(networking::Request &t_req, std::string &t_to_delete) {
  try {
    int items = count_directory_items(t_to_delete);
    if (items == 0) { // dir must have 0 items to be deleted.
      fs::remove_all(t_to_delete);
      t_req.m_reply = networking::reply::r_250;

    } else {
      t_req.m_reply = networking::reply::r_532; // repurposed 532. See readme.
      t_req.m_reply_msg = "Unable to remove. " + join_to_user_path(t_req, t_req.m_argument) +
                          " contains " + std::to_string(items) + " item(s).";
    }
  } catch (std::exception &_err) {
    t_req.m_reply = networking::reply::r_532;
    t_req.m_reply_msg = "Unable to remove. Not a path to a directory.";
  }
}

void DiskManager::change_bad_path(networking::Request &t_req) {
  LOG_ERROR("CWD to unexisting directory");
  t_req.m_valid = false;
  t_req.m_reply = networking::reply::r_550; /* file unavailable */
}

void DiskManager::change_valid_path(networking::Request &t_req) {
  t_req.m_valid = true;
  t_req.m_reply = networking::reply::r_250;
}

void DiskManager::change_absolute_path(networking::Request &t_req) {
  t_req.m_disk.m_user_path = t_req.m_argument;
  t_req.m_disk.m_system_path = M_SYS_PATH + "/" + M_ROOT + t_req.m_argument;
  t_req.m_valid = true;
  t_req.m_reply = networking::reply::r_250;
}

bool DiskManager::path_exists(std::string &t_path) {
  const fs::path p(t_path);
  return fs::exists(p);
}

bool DiskManager::is_absolute_path(std::string &t_path) {
  return t_path[0] == '/';
}

bool DiskManager::is_working_dir_root(networking::Request &t_req) {
  return t_req.m_disk.m_user_path == "/";
}

std::string DiskManager::join_to_user_path(networking::Request &t_req,
                                           std::string path_to_join) {
  return is_working_dir_root(t_req)
             ? t_req.m_disk.m_user_path + path_to_join
             : t_req.m_disk.m_user_path + "/" + path_to_join;
}

std::string DiskManager::join_to_system_path(networking::Request &t_req,
                                             std::string path_to_join) {
  return t_req.m_disk.m_system_path + "/" + path_to_join;
}

std::string DiskManager::create_system_root_path() {
  return M_SYS_PATH + "/" + M_ROOT;
}

void DiskManager::remove_trailing_slash(networking::Request &t_req) {
  if (t_req.m_disk.m_system_path.back() == '/')
    t_req.m_disk.m_system_path.erase(t_req.m_disk.m_system_path.end() - 1,
                                     t_req.m_disk.m_system_path.end());

  /* size() > 1 skips single a single backslash */
  if (t_req.m_disk.m_user_path.back() == '/' &&
      t_req.m_disk.m_user_path.size() > 1)
    t_req.m_disk.m_user_path.erase(t_req.m_disk.m_user_path.end() - 1,
                                   t_req.m_disk.m_user_path.end());
}

int DiskManager::count_resuling_dir_level(utils::StringVector &t_paths) {
  int count = 0;

  for (auto &path : t_paths) {
    if (path == "..")
      count--;

    else if (path == ".")
      continue;

    else
      count++;
  }

  return count;
}

int DiskManager::count_directory_items(std::string &t_path) {
  int count = 0;
  for (auto &_file : fs::directory_iterator(t_path)) {
    count++;
  }
  return count;
}

TEST_CASE("Disk Manager") {

  std::string system_path = fs::current_path();
  std::string system_root_path = system_path + "/" + ".root";

  SUBCASE("change dir to path") {

    Disk disk;
    DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "test";

    DiskManager::change_directory(req);

    CHECK(req.m_reply == networking::reply::r_250);
    CHECK(req.m_disk.m_user_path == "/test");
    CHECK(req.m_disk.m_system_path == system_root_path + "/" + "test");
  }

  SUBCASE("change dir invalid path") {
    Disk disk;
    DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "test/does_not_exist";

    DiskManager::change_directory(req);

    CHECK(req.m_reply == networking::reply::r_550);
  }

  SUBCASE("change dir using .") {
    Disk disk;
    DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "test/.";

    DiskManager::change_directory(req);

    CHECK(req.m_reply == networking::reply::r_250);
    CHECK(req.m_disk.m_user_path == "/test");
    CHECK(req.m_disk.m_system_path == system_root_path + "/" + "test");
  }

  SUBCASE("change dir with multiples . and ..") {
    Disk disk;
    DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "test/./../test/../.";

    DiskManager::change_directory(req);

    CHECK(req.m_reply == networking::reply::r_250);
    CHECK(req.m_disk.m_user_path == "/");
    CHECK(req.m_disk.m_system_path == system_root_path);
  }

  SUBCASE("change dir to root") {
    Disk disk;
    DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "/";

    DiskManager::change_directory(req);

    CHECK(req.m_reply == networking::reply::r_250);
    CHECK(req.m_disk.m_user_path == "/");
    CHECK(req.m_disk.m_system_path == system_root_path);
  }

  SUBCASE("change dir with absolute path") {
    Disk disk;
    DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "/test/inside";

    DiskManager::change_directory(req);

    CHECK(req.m_reply == networking::reply::r_250);
    CHECK(req.m_disk.m_user_path == "/test/inside");
    CHECK(req.m_disk.m_system_path == system_root_path + "/" + "test/inside");
  }

  SUBCASE("Change dir down by then up by two") {
    Disk disk;
    DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "test/inside";

    DiskManager::change_directory(req);

    CHECK(req.m_reply == networking::reply::r_250);
    CHECK(req.m_disk.m_user_path == "/test/inside");
    CHECK(req.m_disk.m_system_path == system_root_path + "/" + "test/inside");

    auto req2 = networking::Request(disk);
    req2.m_argument = "../..";

    DiskManager::change_directory(req2);

    CHECK(req2.m_disk.m_user_path == "/");
    CHECK(req2.m_disk.m_system_path == system_root_path);
  }

  SUBCASE("change dir out of bounds") {
    Disk disk;
    DiskManager::init(disk);
    auto req = networking::Request(disk);
    req.m_argument = "../../..";

    DiskManager::change_directory(req);

    CHECK(req.m_reply == networking::reply::r_550);
  }
}
