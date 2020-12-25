#pragma once

#include <sys/socket.h>

#include <boost/asio.hpp>
#include <string>

class UDPServer {
   public:
    UDPServer(boost::asio::io_context& io_context,
              const boost::asio::ip::address& multicast_address);
    void send();

   private:
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::socket socket_;
};