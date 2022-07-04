#include "data_manager.hpp"
#include "connection.hpp"
#include "disk_manager.hpp"
#include "doctest.h"
#include "logger.hpp"
#include <sstream>
#include <string>

using namespace controllers;

const int DataManager::M_PORT_SPLIT_POS = 4;
const int DataManager::M_PORT_ARG_LEN = 6;

void DataManager::port(networking::Request &t_req,
                       networking::Connection &t_conn) {

  utils::StringVector port_argument =
      utils::StringHelpers::split_string(t_req.m_argument, ",");

  if (port_argument.size() < M_PORT_ARG_LEN) {
    throw port_length_err(port_argument.size());

  } else {

    data_connect(port_argument, t_conn);
    t_req.m_reply = networking::reply::r_200;
  }
}

void DataManager::type(networking::Request &t_req,
                       networking::Connection &t_conn) {

  networking::conn_type type = select_type(t_req.m_argument);

  if (type == networking::none) {
    t_req.m_reply = networking::reply::r_501;
    return;
  }

  t_conn.set_type(type);
  t_req.m_reply = networking::reply::r_200;
}

void DataManager::retrieve(networking::Request &t_req,
                           networking::Connection &t_conn) {

  LOG_DEBUG("Retrieve was called!");
}

void DataManager::list(networking::Request &t_req,
                       networking::Connection &t_conn) {

  if (t_conn.get_type() == networking::acii) {

    t_req.m_reply = networking::reply::r_150;
    t_req.m_isdata = true;
    t_req.m_data.m_ascii = "list of files\n";

  } else {

    int port = t_conn.get_port();
    t_conn = networking::Connection(port, networking::active); /* kill connection */
    t_req.m_reply = networking::reply::r_426;
    t_req.m_reply_msg = " 'TYPE' must be set to ASCII.";
  }

    /* restablish connection after transfer */  
}

void DataManager::data_connect(utils::StringVector &t_port_argument,
                               networking::Connection &t_conn) {

  std::string ip = extract_ip(t_port_argument);
  int port = extract_port(t_port_argument);

  t_conn.set_port(port);
  t_conn.set_socket_options();
  t_conn.config_addr(ip);
  t_conn.connect_socket();
}

std::string DataManager::extract_ip(utils::StringVector &t_port_arg) {

  utils::StringVector ip(t_port_arg.begin(),
                         t_port_arg.begin() + M_PORT_SPLIT_POS);

  return utils::StringHelpers::join_string_vector(ip, ".");
}

int DataManager::extract_port(utils::StringVector &t_port_arg) {

  utils::StringVector port(t_port_arg.begin() + M_PORT_SPLIT_POS,
                           t_port_arg.end());
  std::string hex_port = decimal_to_hex(port[0]) + decimal_to_hex(port[1]);

  return hex_to_decimal(hex_port);
}

std::string DataManager::decimal_to_hex(const std::string t_decimal) {

  std::stringstream hex;
  hex << std::hex << std::stoi(t_decimal);

  return hex.str();
}

int DataManager::hex_to_decimal(const std::string t_hex) {

  std::stringstream hex;
  int decimal;

  hex << t_hex;
  hex >> std::hex >> decimal;

  return decimal;
}

networking::conn_type DataManager::select_type(std::string &t_type) {
  if (t_type == "A")
    return networking::acii;
  else if (t_type == "I")
    return networking::image;
  else
    return networking::none;
}

std::string DataManager::port_length_err(int t_arg_len) {

  std::string err_msg =
      "PORT argument has an incorrect number of fields. It was '";

  err_msg += std::to_string(t_arg_len) + "' ; should be '" +
             std::to_string(M_PORT_ARG_LEN) + "'.";

  return err_msg;
}

// TEST_CASE("Data Manager") {
//
//   disk::Disk disk;
//   DiskManager::init(disk);
//   auto req = networking::Request(disk);
//   req.m_argument = "127,0,0,1,211,181";
//
//   DataManager::port(req);
//
//   LOG_DEBUG("PORT: %d", req.m_port);
// }
