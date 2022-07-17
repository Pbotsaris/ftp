#ifndef DATA_CONNECTION_H
#define DATA_CONNECTION_H
#include "connection.hpp"

namespace networking {

typedef  std::tuple<networking::ImageBuffer, std::uintmax_t>  DatafromClientTuple;

    struct TransferData{
    char *m_buffer;
    std::size_t m_length;
  };

class DataConnection : public Connection {

  using Connection::Connection;
  
public:
  enum conn_type { none, image, ascii };

  void                transfer_send(Request &t_req);
  DatafromClientTuple transfer_receive(Request &t_req);
  conn_type           get_type();
  void                set_type(conn_type t_type);


private:
  conn_type           m_type = ascii; 
  void                transfer_ascii(Request &t_req);
  void                transfer_image(Request &t_req);
  ImageBuffer         consolidate_data(std::queue<TransferData> t_data, std::uintmax_t t_total_length);

};

} // namespace networking

#endif
