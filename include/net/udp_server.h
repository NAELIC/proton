#pragma once

#include <sys/socket.h>

#include <boost/asio.hpp>
#include <string>

class UDPServer {
   public:
    UDPServer(std::string address, unsigned int port,
              boost::asio::io_context& io_context);

    void send();  // TODO : Send Grsim Packet !

   private:
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::socket socket_;
};