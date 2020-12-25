#include "udp_server.h"

using namespace boost;

UDPServer::UDPServer(asio::io_context& io_context,
                     const asio::ip::address& multicast_address)
    : endpoint_(multicast_address, 10020),
      socket_(io_context, endpoint_.protocol()) {
    socket_.set_option(asio::ip::multicast::hops(1));
}

void UDPServer::send() {
    std::string message = "Hello World !";
    system::error_code ignored_error;
    socket_.send_to(asio::buffer(message), endpoint_, 0, ignored_error);
}
