#include "udp_client.h"

using namespace boost;

UDPClient::UDPClient(asio::io_service& io_service,
                     const asio::ip::address& listen_address,
                     const asio::ip::address& multicast_address)
    : socket_(io_service)
{
    asio::ip::udp::endpoint listen_endpoint(listen_address,
                                                   20011);
    socket_.open(listen_endpoint.protocol());
    socket_.set_option(asio::ip::udp::socket::reuse_address(true));
    socket_.bind(listen_endpoint);

    // Join the multicast group.
    socket_.set_option(
        asio::ip::multicast::join_group(multicast_address));

    socket_.async_receive_from(
        boost::asio::buffer(data_, 1024), sender_endpoint_,
        boost::bind(&UDPClient::handle_receive_from, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void UDPClient::handle_receive_from(const boost::system::error_code& error,
      size_t bytes_recvd)
  {
    if (!error)
    {
      std::cout.write(data_, bytes_recvd);
      std::cout << std::endl;

      socket_.async_receive_from(
          boost::asio::buffer(data_, max_length), sender_endpoint_,
          boost::bind(&UDPClient::handle_receive_from, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
  }