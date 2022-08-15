#include "disk_manager.hpp"
#include "logger.hpp"

/* Tests */
#include "doctest.h"
#include "utils_path.hpp"

// stop unused variable warning when looping with `for(auto &some_var :
// some_enumerable)
#define MON_Internal_UnusedStringify(macro_arg_string_literal)                 \
#macro_arg_string_literal
#define MONUnusedParameter(macro_arg_parameter)                                \
  _Pragma(MON_Internal_UnusedStringify(unused(macro_arg_parameter)))

using namespace controllers;
using namespace disk;

#if __linux__
namespace fs = std::filesystem;

#elif __APPLE__ || __MACH__
namespace fs = std::__fs::filesystem;

#endif

/* rename requires to keep state accross request cycles
 * the static variable assists with that
 *  May want to expand into an array for multi user support.
 *
 * */

std::string DiskManager::m_rename_from = "";
std::mutex DiskManager::m_mutex;

void DiskManager::init(Disk &t_disk) {
  t_disk.m_user_path = "/";
  t_disk.m_system_path = utils::PathHelpers::create_system_root_path();
  t_disk.m_dir_level = 0;


  std::lock_guard<std::mutex>lock_guard (m_mutex);
  fs::create_directories(utils::PathHelpers::M_ROOT);
};

/****** File Operations ******/

/** Rename from **/

void DiskManager::rename_from(networking::Request &t_req) {

  std::string rename_from = utils::PathHelpers::build_system_path(t_req);

  std::lock_guard<std::mutex>lock_guard (m_mutex);
  if (utils::PathHelpers::path_exists(rename_from)) {
    m_rename_from = rename_from;
    t_req.m_reply = networking::reply::r_350;
  } else {
    t_req.m_reply = networking::reply::r_550;
  }
};

/** Rename To **/

void DiskManager::rename_to(networking::Request &t_req) {

  std::lock_guard<std::mutex>lock_guard (m_mutex);
  std::string rename_to = utils::PathHelpers::build_system_path(t_req);

  try {
    fs::rename(m_rename_from, rename_to);
  } catch (std::exception &_err) {
    t_req.m_reply = networking::reply::r_450;
    return;
  }

  t_req.m_reply = networking::reply::r_250;
}

/** Delete File **/

void DiskManager::delete_file(networking::Request &t_req) {

  std::lock_guard<std::mutex>lock_guard (m_mutex);
  std::string path_to_del = utils::PathHelpers::add_to_path(
      t_req.m_disk.m_system_path, t_req.m_argument);

  if (fs::remove(path_to_del))
    t_req.m_reply = networking::reply::r_250;
  else
    t_req.m_reply = networking::reply::r_550;
}

/****** Directory Operations ******/

/** Change Directory **/

void DiskManager::change_directory(networking::Request &t_req) {

  if (utils::PathHelpers::is_absolute_path(t_req.m_argument)) {
    change_absolute_path(t_req);
    remove_trailing_slash(t_req);
    return;
  }

  utils::StringVector paths =
      utils::StringHelpers::split_string(t_req.m_argument, "/");

  int resulting_dir_level = count_resuling_dir_level(paths);

  if (t_req.m_disk.m_dir_level + resulting_dir_level < 0) {
    change_bad_path(t_req);
    return;
  }

  update_paths(t_req, paths);
  remove_trailing_slash(t_req);
  t_req.m_disk.m_dir_level = t_req.m_disk.m_dir_level + resulting_dir_level;
}

/** Change Up Directory **/

void DiskManager::change_up_directory(networking::Request &t_req) {
  t_req.m_argument = "..";
  change_directory(t_req);
}

/** Print Working Directory **/

void DiskManager::print_working_directory(networking::Request &t_req) {

  t_req.m_reply = networking::reply::r_257;
  t_req.m_reply_msg = t_req.m_disk.m_user_path;
}

void DiskManager::make_directory(networking::Request &t_req) {

  std::lock_guard<std::mutex>lock_guard (m_mutex);
  if (utils::PathHelpers::is_absolute_path(t_req.m_argument)) {
    fs::create_directories(utils::PathHelpers::create_system_root_path() +
                           t_req.m_argument);
    t_req.m_reply_msg = t_req.m_argument + " created.";

  } else {
    fs::create_directories(
        utils::PathHelpers::join_to_system_path(t_req, t_req.m_argument));
    t_req.m_reply_msg =
        utils::PathHelpers::join_to_user_path(t_req, t_req.m_argument) +
        " created.";
  }
  t_req.m_reply = networking::reply::r_257;
}

/** Remove Directory **/

void DiskManager::remove_directory(networking::Request &t_req) {

  auto to_delete = utils::PathHelpers::build_system_path(t_req);

  std::lock_guard<std::mutex>lock_guard (m_mutex);
  if (utils::PathHelpers::path_exists(to_delete)) {
    remove_directory_when_valid(t_req, to_delete);
  } else {
    t_req.m_reply = networking::reply::r_550;
  }
}

/*** PRIVATE ***/

void DiskManager::update_paths(networking::Request &t_req,
                               const utils::StringVector &t_paths) {

  std::string user_path = t_req.m_disk.m_user_path;
  std::string system_path = t_req.m_disk.m_system_path;

  for (auto &path : t_paths) {

    if (path == "..") {
      user_path = utils::PathHelpers::remove_last_path(user_path);
      system_path = utils::PathHelpers::remove_last_path(system_path);
    } else if (path == ".") {
      continue;
    }

    else {
      user_path = utils::PathHelpers::add_to_path(user_path, path);
      system_path = utils::PathHelpers::add_to_path(system_path, path);
    }
  }

  if (utils::PathHelpers::path_exists(system_path)) {
    t_req.m_disk.m_user_path = user_path;
    t_req.m_disk.m_system_path = system_path;
    change_valid_path(t_req);
  } else {

    change_bad_path(t_req);
  }
}

/** **/

void DiskManager::remove_directory_when_valid(networking::Request &t_req,
                                              std::string &t_to_delete) {

  try {
    int items = count_directory_items(t_to_delete);
    if (items == 0) { // dir must have 0 items to be deleted.
      fs::remove_all(t_to_delete);
      t_req.m_reply = networking::reply::r_250;

    } else {
      t_req.m_reply = networking::reply::r_532; // repurposed 532. See readme.
      t_req.m_reply_msg =
          "Unable to remove. " +
          utils::PathHelpers::join_to_user_path(t_req, t_req.m_argument) +
          " contains " + std::to_string(items) + " item(s).";
    }
  } catch (std::exception &_err) {
    t_req.m_reply = networking::reply::r_532;
    t_req.m_reply_msg = "Unable to remove. Not a path to a directory.";
  }
}

/** **/

void DiskManager::change_bad_path(networking::Request &t_req) {
  LOG_ERROR("CWD to unexisting directory");
  t_req.m_reply = networking::reply::r_550; /* file unavailable */
}

/** **/

void DiskManager::change_valid_path(networking::Request &t_req) {
  t_req.m_reply = networking::reply::r_250;
}

/** **/

void DiskManager::change_absolute_path(networking::Request &t_req) {
  t_req.m_disk.m_user_path = t_req.m_argument;
  t_req.m_disk.m_system_path = utils::PathHelpers::M_SYS_PATH + "/" +
                               utils::PathHelpers::M_ROOT + t_req.m_argument;
  t_req.m_reply = networking::reply::r_250;
}

/** **/

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

/** **/

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

/** **/

int DiskManager::count_directory_items(std::string &t_path) {
  int count = 0;
  for (auto &_file : fs::directory_iterator(t_path)) {
    MONUnusedParameter(_file);

    count++;
  }
  return count;
}

/** TESTS **/

TEST_CASE("Disk Manager") {

  std::string system_path = fs::current_path();
  std::string system_root_path = system_path + "/" + ".root";

  // make directories for testing
  fs::create_directories(utils::PathHelpers::M_ROOT + "/test");
  fs::create_directories(utils::PathHelpers::M_ROOT + "/test/inside");

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

  SUBCASE("Change dir down one by then up by two") {
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

  /* remove after tests */
  fs::remove_all(utils::PathHelpers::M_ROOT + "./test/inside");
  fs::remove_all(utils::PathHelpers::M_ROOT + "/test");
}
