#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "request.hpp"
#include "connection.hpp"
#include "utils_string.hpp"

namespace controllers {

struct DataManager {

  static const int M_PORT_SPLIT_POS;
  static const int M_PORT_ARG_LEN;

  static void port(networking::Request &t_req, networking::Connection &t_conn);
  static void get(networking::Request &t_req, networking::Connection &t_conn);
  static void list(networking::Request &t_req, networking::Connection &t_conn);

  private:
  static void data_connect(utils::StringVector &t_port_argument, networking::Connection &t_conn);

  static std::string extract_ip(utils::StringVector &t_port_arg);
  static int extract_port(utils::StringVector &t_port_arg);

  static std::string decimal_to_hex(const std::string t_decimal);
  static int hex_to_decimal(const std::string t_hex);

  /* exception messages */
  static std::string port_length_err(int t_arg_len);

};

} // namespace controllers

#endif // !DATA_MANAGER_H
