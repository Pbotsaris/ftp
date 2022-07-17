#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "request.hpp"
#include "data_connection.hpp"
#include "utils_file.hpp"
#include "utils_string.hpp"

namespace controllers {

struct DataManager {

  using ConnectionType = networking::DataConnection::conn_type;
  typedef std::tuple<int, std::string> PortTuple;

  static const int M_PORT_SPLIT_POS;
  static const int M_PORT_ARG_LEN;
  static const int M_RANDOM_FILENAME_LENGTH;
  static const int M_MIN_P1;
  static const int M_MAX_P1;
  static const int M_MIN_P2;
  static const int M_MAX_P2;

  static void                  port(networking::Request &t_req, networking::DataConnection &t_conn);
  static void                  retrieve(networking::Request &t_req, networking::DataConnection &t_conn);
  static void                  list(networking::Request &t_req, networking::DataConnection &t_conn);
  static void                  list_names(networking::Request &t_req, networking::DataConnection &t_conn);
  static void                  type(networking::Request &t_req, networking::DataConnection &t_conn);
  static void                  store(networking::Request &t_req, networking::DataConnection &t_conn);
  static void                  store_unique(networking::Request &t_req, networking::DataConnection &t_conn);
  static void                  passive(networking::Request &t_req, networking::DataConnection &t_conn);

  private:

  /* Port helpers */
  static void                  data_connect(utils::StringVector &t_port_argument, networking::DataConnection &t_conn);

  /* list helpers */
  static void                  valid_to_list(networking::Request &t_req);
  static void                  invalid_to_list(networking::Request &t_req, networking::DataConnection &t_conn);
  static void                  list_with_argument(networking::Request &t_req, utils::FileHelpers::listdir_option t_lopt);

  /* retreive helpers */
  static void                  valid_to_retrieve(networking::Request &t_req);
  static void                  invalid_to_retrieve(networking::Request &t_req, networking::DataConnection &t_conn);

  /* Store helpers */
  static void                  valid_to_store(networking::Request &t_req);
  static void                  invalid_to_store(networking::Request &t_req, networking::DataConnection &t_conn);

  /* port helpers */
  static std::string           extract_ip(utils::StringVector &t_port_arg);
  static int                   extract_port(utils::StringVector &t_port_arg);
  static PortTuple             generate_port();

  /* Other */
  static ConnectionType        select_type(std::string &t_type);
  static std::string           port_length_err(int t_arg_len);
};

} // namespace controllers

#endif // !DATA_MANAGER_H
