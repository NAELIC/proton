#pragma once

#include <sys/socket.h>

#include <boost/asio.hpp>
#include <string>

#include "messages_robocup_ssl_robot_status.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"
class UDPServer {
   public:
    UDPServer(std::string multicast_address, unsigned int port,
              boost::asio::io_context& io_context);

    UDPServer(unsigned int port, boost::asio::io_context& io_context);

    void send(const SSL_WrapperPacket* packet);
    void send(const Robots_Status* packet);

   private:
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::socket socket_;
};