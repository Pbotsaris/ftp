#include "data_manager.hpp"
#include "connection.hpp"
#include "disk_manager.hpp"
#include "doctest.h"
#include "logger.hpp"
#include "utils_file.hpp"
#include "utils_string.hpp"
#include <sstream>
#include <string>

using namespace controllers;
 
/* constants to split IP addres */
const int DataManager::M_PORT_SPLIT_POS = 4;
const int DataManager::M_PORT_ARG_LEN = 6;
const int DataManager::M_RANDOM_FILENAME_LENGTH = 10;

/***** Callbacks ********/

void DataManager::port(networking::Request &t_req,
                       networking::Connection &t_conn) {

  utils::StringVector port_argument =
      utils::StringHelpers::split_string(t_req.m_argument, ",");

  if (port_argument.size() < M_PORT_ARG_LEN) {
    LOG_ERROR(port_length_err(port_argument.size()).c_str());
    t_req.m_reply = networking::reply::r_500;

  } else {
    data_connect(port_argument, t_conn);
    t_req.m_reply = networking::reply::r_200;
  }
}

/** **/

void DataManager::type(networking::Request &t_req, networking::Connection &t_conn) {

  networking::Connection::conn_type type = select_type(t_req.m_argument);

  if (type == networking::Connection::none) {
    t_req.m_reply = networking::reply::r_501;
    return;
  }

  t_conn.set_type(type);
  t_req.m_reply = networking::reply::r_200;
}

/** **/

void DataManager::retrieve(networking::Request &t_req, networking::Connection &t_conn) {

  t_conn.set_type(networking::Connection::image);
  LOG_INFO("Setting Transfer Type to Binary/Image.");

  if (t_req.m_argument.empty())
    invalid_to_retrieve(t_req, t_conn);

  else
    valid_to_retrieve(t_req);
}

/** **/

void DataManager::list(networking::Request &t_req, networking::Connection &t_conn) {

  t_conn.set_type(networking::Connection::ascii);
  LOG_INFO("Setting Transfer Type to ASCII.");

  try {
    valid_to_list(t_req);

  } catch (std::string &err) {

    LOG_ERROR(err.c_str());
    invalid_to_list(t_req, t_conn);
  }
}

/** **/

void DataManager::store(networking::Request &t_req, networking::Connection &t_conn) {

  t_conn.set_type(networking::Connection::image);
  LOG_INFO("Setting Transfer Type to Binary/Image.");

  if (t_req.m_argument.empty())
    invalid_to_store(t_req, t_conn);

  else
    valid_to_store(t_req);
}

/** **/

void DataManager::store_unique(networking::Request &t_req, networking::Connection &t_conn) {

  t_req.m_argument = utils::StringHelpers::random_string(M_RANDOM_FILENAME_LENGTH);
  store(t_req, t_conn);
}


/** **/

void DataManager::passive(networking::Request &t_req, networking::Connection &t_conn) {

  /* TODO: IMPLEMENT PASSIVE MODE */
  t_req.m_reply = networking::reply::r_228;
}

/**** PRIVATE *****/

void DataManager::data_connect(utils::StringVector &t_port_argument,
                               networking::Connection &t_conn) {

  std::string ip = extract_ip(t_port_argument);
  int port = extract_port(t_port_argument);

  t_conn.set_port(port);
  t_conn.config_addr(ip);
  t_conn.connect_socket();
}

/**** LIST ****/

void DataManager::valid_to_list(networking::Request &t_req) {

  t_req.m_reply = networking::reply::r_150;
  t_req.m_transfer = networking::Request::send;

  if (t_req.m_argument.empty())
    t_req.m_data.m_ascii = utils::FileHelpers::list_dir_filenames( t_req, utils::FileHelpers::list_stat);

  else
    list_with_argument(t_req);
}

/** **/

void DataManager::invalid_to_list(networking::Request &t_req, networking::Connection &t_conn) {

  t_conn.reconnect();
  t_req.m_valid = false;
  t_req.m_reply = networking::reply::r_550;
}

void DataManager::list_with_argument(networking::Request &t_req) {

  std::string path = utils::PathHelpers::join_to_system_path(t_req, t_req.m_argument);

  if (utils::PathHelpers::is_path_directory(path))
    t_req.m_data.m_ascii = utils::FileHelpers::list_dir_filenames( t_req, utils::FileHelpers::list_stat);

  else
    t_req.m_data.m_ascii = utils::FileHelpers::stat_file(t_req);
}

/**** RETRIEVE ****/

void DataManager::valid_to_retrieve(networking::Request &t_req) {

  try {
    utils::FileHelpers::DataFromDiskTuple alloced =
        utils::FileHelpers::read_bytes(t_req);

    t_req.m_data.m_image = std::move(std::get<0>(alloced));
    t_req.m_data.m_image_size = std::get<1>(alloced);

    t_req.m_reply = networking::reply::r_150;
    t_req.m_transfer = networking::Request::send;

  } catch (std::string &err) {

    LOG_ERROR(err.c_str());
    t_req.m_reply = networking::reply::r_426;
    t_req.m_reply_msg = err;
  }
}


/** **/

void DataManager::invalid_to_retrieve(networking::Request &t_req, networking::Connection &t_conn) {

  LOG_ERROR("retrieve was called without any arguments.");
  t_req.m_reply = networking::reply::r_501;
  t_req.m_valid = false;
  t_conn.reconnect();
}

/**** STORE ****/

void DataManager::valid_to_store(networking::Request &t_req) {

  t_req.m_reply = networking::reply::r_125;
  t_req.m_reply_msg = "FILE: " + t_req.m_argument;
  t_req.m_transfer = networking::Request::receive;
}

void DataManager::invalid_to_store(networking::Request &t_req, networking::Connection &t_conn) {

  LOG_ERROR("retrieve was called without any arguments.");
  t_req.m_reply = networking::reply::r_501;
  t_req.m_valid = false;
  t_conn.reconnect();
}

/**** HELPERS ****/

std::string DataManager::extract_ip(utils::StringVector &t_port_arg) {

  utils::StringVector ip(t_port_arg.begin(),
                         t_port_arg.begin() + M_PORT_SPLIT_POS);

  return utils::StringHelpers::join_string_vector(ip, ".");
}

/** **/

int DataManager::extract_port(utils::StringVector &t_port_arg) {

  utils::StringVector port(t_port_arg.begin() + M_PORT_SPLIT_POS,
                           t_port_arg.end());
  std::string hex_port = decimal_to_hex(port[0]) + decimal_to_hex(port[1]);

  return hex_to_decimal(hex_port);
}

/** **/

std::string DataManager::decimal_to_hex(const std::string t_decimal) {

  std::stringstream hex;
  hex << std::hex << std::stoi(t_decimal);

  return hex.str();
}


/** **/

int DataManager::hex_to_decimal(const std::string t_hex) {

  std::stringstream hex;
  int decimal;

  hex << t_hex;
  hex >> std::hex >> decimal;

  return decimal;
}


/** **/

networking::Connection::conn_type
DataManager::select_type(std::string &t_type) {
  if (t_type == "A")
    return networking::Connection::ascii;
  else if (t_type == "I")
    return networking::Connection::image;
  else
    return networking::Connection::none;
}

std::string DataManager::port_length_err(int t_arg_len) {

  std::string err_msg =
      "PORT argument has an incorrect number of fields. It was '";

  err_msg += std::to_string(t_arg_len) + "' ; should be '" +
             std::to_string(M_PORT_ARG_LEN) + "'.";

  return err_msg;
}
