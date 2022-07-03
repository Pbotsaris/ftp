#include "data_manager.hpp"
#include "logger.hpp"
#include <sstream>
#include <string>

using namespace controllers;

const int DataManager::M_PORT_SPLIT_POS = 4;
const int DataManager::M_PORT_ARG_LEN = 6;

void DataManager::port(networking::Request &t_req) {

  utils::StringVector split_port_arg =
      utils::StringHelpers::split_string(t_req.m_argument, ",");

  if (split_port_arg.size() < M_PORT_ARG_LEN)
    throw port_length_err(split_port_arg.size());

        std::string ip = extract_ip(split_port_arg);
  std::string port = extract_port(split_port_arg);

  LOG_DEBUG(ip.c_str());
  LOG_DEBUG(port.c_str());
}

std::string DataManager::extract_ip(utils::StringVector &t_port_arg) {

  utils::StringVector ip(t_port_arg.begin(),
                         t_port_arg.begin() + M_PORT_SPLIT_POS);

  return utils::StringHelpers::join_string_vector(ip, ".");
}

std::string DataManager::extract_port(utils::StringVector &t_port_arg) {

  utils::StringVector port(t_port_arg.begin() + M_PORT_SPLIT_POS,
                           t_port_arg.end());

  std::string hex_port = decimal_to_hex(port[0]) + decimal_to_hex(port[1]);

  return hex_to_decimal(hex_port);
}

std::string DataManager::decimal_to_hex(const std::string t_decimal) {

  std::stringstream hex;
  hex << std::hex << t_decimal;

  return hex.str();
}

std::string DataManager::hex_to_decimal(const std::string t_hex) {

  std::stringstream hex;
  std::string decimal;

  hex << t_hex;
  hex >> std::hex >> decimal;

  return decimal;
}

std::string DataManager::port_length_err(int t_arg_len) {

  std::string err_msg =
      "PORT argument has an incorrect number of fields. It was '";

  err_msg += std::to_string(t_arg_len) + "' ; should be '" +
             std::to_string(M_PORT_ARG_LEN) + "'.";

  return err_msg;
}
