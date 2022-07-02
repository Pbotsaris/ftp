#ifndef UTILS_FILE_H
#define UTILS_FILE_H
#include "request.h"
#include "utils_path.h"
#include <filesystem>
#include <fstream>

namespace utils {

/* NOTE: FileHelpers functions are pure; will never modifies arguments.*/


struct FileHelpers {

  static std::string list_dir_filenames(const networking::Request &t_req);
};

} // namespace utils

#endif // !UTILS_FILE_H
