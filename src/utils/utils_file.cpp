#include "utils_file.hpp"
#include "logger.hpp"

#if __linux__ 
namespace fs = std::filesystem;

#elif __APPLE__ || __MACH__
namespace fs = std::__fs::filesystem;

#endif

using namespace utils;

std::string FileHelpers::list_dir_filenames(const networking::Request &t_req){

    const std::string path_to_list = utils::PathHelpers::join_to_system_path(t_req, t_req.m_argument);
    std::string filenames;

     if (!utils::PathHelpers::path_exists(path_to_list)) 
       throw "Failed listing directory; The path '" + utils::PathHelpers::join_to_user_path(t_req, t_req.m_argument) + "' does not exist.";
    
    for (const auto & entry : fs::directory_iterator(path_to_list)) {
        filenames.append(utils::PathHelpers::extract_last_path(entry.path()) + " ");
    }

    return filenames;
}

