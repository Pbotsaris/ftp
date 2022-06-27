//#include "../../include/logger.h"
//
////template <typename... FormatArgs>
////void Logger::log(SrcCodeInfo t_src_info, char* t_msg, FormatArgs... t_args) {
////
////  const char *file;
////  const char *func;
////  int line;
////
////  std::tie(file, func, line) = t_src_info;
////  const char *last_path = last_pathname(file);
////
////  std::cout << " <" << last_path << "> " << func << ":" << line;
////  std::printf(t_msg, t_args...);
////  std::cout << std::endl;
////}
//
//template <typename... FormatArgs>
//void Logger::log2(const char *f, const char *fun, int l, const char *msg,
//                  FormatArgs... t_args){
//
//
////  const char *last_path = last_pathname(f);
////
////  std::cout << " <" << last_path << "> " << f << ":" << l;
////  std::printf(msg, t_args...);
//
//}
//
//const char *Logger::last_pathname(const char *t_file, int c) {
//
//  return std::strrchr(t_file, c) ? std::strrchr(t_file, c) + 1 : t_file;
//}
//
//
