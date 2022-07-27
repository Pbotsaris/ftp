#include "control_io.hpp"
#include "connection_poll.hpp"
#include "logger.hpp"

static const int BUFFER_SIZE = 100;
static const int MAX_READ_SIZE = BUFFER_SIZE - 2;

using namespace networking;

ControlIO::ControlIO(): m_socket(-1){}

ControlIO::ControlIO(int t_socket): m_socket(t_socket){}

bool ControlIO::receive(Request &t_req) {

  char read_buffer[BUFFER_SIZE] = {0};
  int read_count = 0;
  ConnectionPoll conn_poll;
     
  while (1) {

    int read_size = recv(m_socket, &read_buffer[read_count], 1, 0);

    if (read_size == 0) {
      LOG_INFO("Nothing to receive from socket %d", m_socket);
      t_req.m_raw = "";
      return false;
    }

    if (read_count >= MAX_READ_SIZE) {
      t_req.m_raw.append(read_buffer);
      memset(read_buffer, 0, BUFFER_SIZE);
      read_count = 0;
    }

    if (read_size < 0) {
      LOG_INFO("There was an error receiving from %d", m_socket);
      t_req.m_raw = "";
      return false;
    }

    if (read_buffer[read_count] == '\n')
      break;

    read_count++;
  }

  t_req.m_raw.append(read_buffer);

  LOG_DEBUG("Message Received: %s", t_req.m_raw.c_str());

  return true;
}

bool ControlIO::respond(Request &t_req) {

  std::string msg;

  if (t_req.m_reply_msg.empty()) {
    msg = reply::messages[t_req.m_reply];
  } else {
    msg = reply::Utils::append_message(t_req.m_reply, t_req.m_reply_msg);
  }

  int res = send(m_socket, msg.c_str(), msg.size(), 0);

  if (res < 0) {

    LOG_ERROR("Could not respond to client\n");
    return false;
  }

  return true;
}

int ControlIO::get_socket(){ return m_socket; }
