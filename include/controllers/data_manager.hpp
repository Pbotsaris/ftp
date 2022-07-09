#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "request.hpp"
#include "connection.hpp"
#include "utils_string.hpp"

namespace controllers {

struct DataManager {

  static const int M_PORT_SPLIT_POS;
  static const int M_PORT_ARG_LEN;

  /* callbacks */
  static void                   port(networking::Request &t_req, networking::Connection &t_conn);
  static void                   retrieve(networking::Request &t_req, networking::Connection &t_conn);
  static void                   list(networking::Request &t_req, networking::Connection &t_conn);
  static void                   type(networking::Request &t_req, networking::Connection &t_conn);
  static void                   store(networking::Request &t_req, networking::Connection &t_conn);
  static void                   passive(networking::Request &t_req, networking::Connection &t_conn);

  private:
  static void                   data_connect(utils::StringVector &t_port_argument, networking::Connection &t_conn);

  /* list helpers */
  static void                   valid_to_list(networking::Request &t_req);
  static void                   invalid_to_list(networking::Request &t_req, networking::Connection &t_conn);
  static void                   list_with_argument(networking::Request &t_req);

  /* retreive helpers */
  static void                  valid_to_retrieve(networking::Request &t_req);
  static void                  invalid_to_retrieve(networking::Request &t_req, networking::Connection &t_conn);

  /* Store helpers */
  static void                  valid_to_store(networking::Request &t_req);
  static void                  invalid_to_store(networking::Request &t_req, networking::Connection &t_conn);

  /* port helpers */
  static std::string           extract_ip(utils::StringVector &t_port_arg);
  static int                   extract_port(utils::StringVector &t_port_arg);
  static std::string           decimal_to_hex(const std::string t_decimal);
  static int                   hex_to_decimal(const std::string t_hex);
  static networking::Connection::conn_type select_type(std::string &t_type);

  /* exception messages */
  static std::string           port_length_err(int t_arg_len);

};

} // namespace controllers

#endif // !DATA_MANAGER_H
