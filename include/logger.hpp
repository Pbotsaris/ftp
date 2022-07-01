#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <map>
#include <stdlib.h>
#include <tuple>
#include <string.h>

#define DEBUG  /* remove this macro remove debug logs */

#define LOG_INFO(...)                                                    \
  logger::Logger::log(std::make_tuple(__FILE__, __FUNCTION__, __LINE__),       \
                      logger::info,  __VA_ARGS__)

#define LOG_ERROR(...)                                                   \
  logger::Logger::log(std::make_tuple(__FILE__, __FUNCTION__, __LINE__),       \
                      logger::error,  __VA_ARGS__)

#ifdef DEBUG
#define LOG_DEBUG(...)                                                   \
  logger::Logger::log(std::make_tuple(__FILE__, __FUNCTION__, __LINE__),       \
                      logger::debug,  __VA_ARGS__)
#else
#define LOG_DEBUG(format...)         
#endif

namespace logger {

typedef std::tuple<const char *, const char *, int> SrcCodeInfo;

enum log_type { info, error, debug };

static const char *RESET_COLOR = "\033[0m";

static const std::map<log_type, const char *> COLOR = {
    {info, "\033[1m\033[34m"},
    {debug, "\033[33m"},
    {error, "\033[1m\033[31m"},
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

// ignore printf format error
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
    std::printf(t_msg, t_args...);
#pragma clang diagnostic pop
    std::cout << RESET_COLOR << std::endl;
  }

private:
  static const char *last_pathname(const char *t_file, int c = '/') {
    return strrchr(t_file, c) ? strrchr(t_file, c) + 1 : t_file;
  }
};
} // namespace logger

#endif
