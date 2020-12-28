#include "net/udp_server.h"

using namespace boost;

UDPServer::UDPServer(std::string address, unsigned int port,
                     asio::io_context& io_context)
    : endpoint_(asio::ip::make_address(address), port),
      socket_(io_context, endpoint_.protocol()) {
    socket_.set_option(asio::ip::multicast::hops(1));
}

UDPServer::UDPServer(unsigned int port, asio::io_context& io_context)
    : endpoint_(asio::ip::udp::v4(), port),
      socket_(io_context, endpoint_.protocol()) {}

void UDPServer::send(const SSL_WrapperPacket* packet) {
    system::error_code ignored_error;

    boost::asio::streambuf b;

    std::ostream os(&b);
    packet->SerializeToOstream(&os);
    socket_.send_to(b.data(), endpoint_, 0, ignored_error);
}

void UDPServer::send(const Robots_Status* packet) {
    system::error_code ignored_error;

    boost::asio::streambuf b;

    std::ostream os(&b);
    packet->SerializeToOstream(&os);
    socket_.send_to(b.data(), endpoint_, 0, ignored_error);
}
