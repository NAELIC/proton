#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <string>


class UDPClient {
   public:
    UDPClient(boost::asio::io_service& io_service,
              const boost::asio::ip::address& listen_address,
              const boost::asio::ip::address& multicast_address);

    void handle_receive_from(const boost::system::error_code& error,
                                        size_t bytes_recvd);

   private:
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint sender_endpoint_;
    enum { max_length = 1024 };
    char data_[max_length];
};