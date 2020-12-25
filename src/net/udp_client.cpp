#include "net/udp_client.h"

using namespace boost;

UDPClient::UDPClient(std::string listen_addr, std::string multicast_addr,
                     unsigned int port, asio::io_context& io_context)
    : socket_(io_context) {
    // Create the socket so that multiple may be bound to the same address.
    asio::ip::udp::endpoint listen_endpoint(asio::ip::make_address(listen_addr),
                                            port);
    socket_.open(listen_endpoint.protocol());
    socket_.set_option(asio::ip::udp::socket::reuse_address(true));
    socket_.bind(listen_endpoint);

    // Join the multicast group.
    socket_.set_option(asio::ip::multicast::join_group(
        asio::ip::make_address(multicast_addr)));

    do_receive();
}

void UDPClient::do_receive() {
    socket_.async_receive_from(
        asio::buffer(data_), sender_endpoint_,
        [this](system::error_code ec, std::size_t length) {
            if (!ec) {
                std::cout.write(data_.data(), length);
                std::cout << std::endl;

                do_receive();
            }
        });
}
