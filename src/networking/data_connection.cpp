#include"data_connection.hpp"
#include "connection.hpp"
#include "logger.hpp"
#include "connection_poll.hpp"

using namespace networking;

/* Buffer size which data is read from socket in passive mode */
static const int TRANSFER_READ_LENGTH = 1048; // 1MB

void DataConnection::transfer_send(Request &t_req) {

  if (m_type == DataConnection::ascii) {
    transfer_ascii(t_req);
  } else {
    transfer_image(t_req);
  }

  LOG_INFO("Closing data connection.");
  reconnect();
  set_mode(active); // reset to active always
}

/** **/

DatafromClientTuple DataConnection::transfer_receive(Request &t_req) {

  std::uintmax_t total_length = 0;
  std::queue<TransferData> queue;

  // passive mode will use the connected socket from accept
  int socket = get_mode() == networking::active ?  get_local_socket() : get_connected_socket();

  ConnectionPoll conn_poll(socket, 300); // wait 300 ms to receive

  while (1) {

    if (conn_poll.has_timeout( "connection")) { /* data connection should never block */
      t_req.m_valid = false;
      break;
    }

    TransferData data;

    data.m_buffer = new char[TRANSFER_READ_LENGTH]();

    int socket      = get_mode() == networking::active ?  get_local_socket() : get_connected_socket();
    int read_length = recv(socket, data.m_buffer, TRANSFER_READ_LENGTH, 0);

    if (read_length == -1) {
      t_req.m_valid = false;
      LOG_ERROR("There was a problem reading from client.");
      delete[] data.m_buffer;
      break;
    }

    data.m_length = static_cast<std::uintmax_t>(read_length);

    /* recv returns 0 when client brakes connection */
    if (data.m_length == 0) {
      delete[] data.m_buffer;
      break;
    }

    total_length += data.m_length;
    queue.push(std::move(data));
  }

  LOG_DEBUG("Read %lu bytes from %s.", total_length, t_req.m_argument.c_str());

  ImageBuffer data = consolidate_data(queue, total_length);

  create_socket();
  set_mode(active); // reset to active always

  return DatafromClientTuple(std::move(data), total_length);
}

void DataConnection::set_type(conn_type t_type) { m_type = t_type; }

DataConnection::conn_type DataConnection::get_type() { return m_type; }


/** Private **/

void DataConnection::transfer_ascii(Request &t_req) {

  int socket = get_mode() == networking::active ?  get_local_socket() : get_connected_socket();
  int res    = send(socket, t_req.m_data.m_ascii.c_str(), t_req.m_data.m_ascii.size(), 0);

  if (res < 0) {
    LOG_ERROR("Could not transfer ASCII data to client.");
    t_req.m_valid = false;
  }
}

/** **/

void DataConnection::transfer_image(Request &t_req) {

  int socket = get_mode() == networking::active ?  get_local_socket() : get_connected_socket();

  int res = send(socket, t_req.m_data.m_image.get(),
                 t_req.m_data.m_image_size, 0);

  if (res < 0) {
    LOG_ERROR("Could not transfer Image/Binary data to client.");
    t_req.m_valid = false;
  }
}

/** **/

ImageBuffer DataConnection::consolidate_data(std::queue<TransferData> t_data, std::uintmax_t t_total_length) {

  ImageBuffer consolidated_data(new char[t_total_length]);
  std::size_t cursor = 0;

  while (!t_data.empty()) {
    TransferData trans_data = t_data.front();
    t_data.pop();
    memmove(consolidated_data.get() + cursor, trans_data.m_buffer,
            trans_data.m_length);
    delete[] trans_data.m_buffer;
    cursor += trans_data.m_length;
  };

  return consolidated_data;
}

