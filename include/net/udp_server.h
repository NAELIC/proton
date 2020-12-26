#pragma once

#include <sys/socket.h>

#include <boost/asio.hpp>
#include <string>

#include "messages_robocup_ssl_wrapper.pb.h"

class UDPServer {
   public:
    UDPServer(std::string address, unsigned int port,
              boost::asio::io_context& io_context);

    void send(const SSL_WrapperPacket* packet);

   private:
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::socket socket_;
};