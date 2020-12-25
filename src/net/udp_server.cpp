#include "udp_server.h"

UDPServer::UDPServer(boost::asio::io_context& io_context,
                     const boost::asio::ip::address& multicast_address)
    : endpoint_(multicast_address, 10020),
      socket_(io_context, endpoint_.protocol()) {}

void UDPServer::send() {
    std::string message = "Hello World !";
    boost::system::error_code ignored_error;
    socket_.send_to(boost::asio::buffer(message), endpoint_, 0, ignored_error);
}
