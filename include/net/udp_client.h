#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <queue>
#include <string>

#include "grSim_Packet.pb.h"

class UDPClient {
   public:
    UDPClient(std::string listen_addr, std::string multicast_addr,
              unsigned int port, boost::asio::io_context& io_context);
    void do_receive();

    std::queue<grSim_Packet> packets;

   private:
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint sender_endpoint_;
    std::array<char, 1024> data_;
};