#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <map>
#include <stdlib.h>
#include <tuple>

#define DEBUG  /* remove this macro remove debug logs */

#define LOG_INFO(format...)                                                    \
  logger::Logger::log(std::make_tuple(__FILE__, __FUNCTION__, __LINE__),       \
                      logger::info, format)

#define LOG_ERROR(format...)                                                   \
  logger::Logger::log(std::make_tuple(__FILE__, __FUNCTION__, __LINE__),       \
                      logger::error, format)

#ifdef DEBUG
#define LOG_DEBUG(format...)                                                   \
  logger::Logger::log(std::make_tuple(__FILE__, __FUNCTION__, __LINE__),       \
                      logger::debug, format)
#else
#define LOG_DEBUG(format...)         
#endif

namespace logger {
typedef std::tuple<const char *, const char *, int> SrcCodeInfo;

static const char *RESET_COLOR = "\033[0m";
enum log_type { info, error, debug };

static const std::map<log_type, const char *> COLOR = {
    {info, "\033[1m\033[34m"},
    {error, "\033[1m\033[31m"},
    {debug, "\033[33m"}
};

static const std::map<log_type, const char *> LABEL = {
    {info, "Info"},
    {error, "Error"},
    {debug, "Debug"}
};


class Logger {

public:
  template <typename... FormatArgs>
  static void log(SrcCodeInfo t_src_info, log_type t_type, const char *t_msg,
                  FormatArgs... t_args) {

    const char* file;
    const char* func;
    const char* color = COLOR.at(t_type);
    const char* label = LABEL.at(t_type);

    int line;

    std::tie(file, func, line) = t_src_info;
    const char *last_path = last_pathname(file);

    std::cout << color << "<" << last_path << " [" << func << ":" << line
              << "]>" << " |" << label <<  "| " ;
    std::printf(t_msg, t_args...);
    std::cout << RESET_COLOR << std::endl;
  }

private:
  static const char *last_pathname(const char *t_file, int c = '/') {
    return std::strrchr(t_file, c) ? std::strrchr(t_file, c) + 1 : t_file;
  }
};
} // namespace logger

#endif
