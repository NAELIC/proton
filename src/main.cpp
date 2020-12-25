#include <unistd.h>

#include <boost/asio.hpp>
#include <csignal>
#include <iostream>
#include <string>

#include "net/udp_client.h"
#include "net/udp_server.h"

bool running = true;

void signalHandler(int signum) {
    running = false;
}

void test_server(boost::asio::io_context& io_context) {
    unsigned int port_vision = 10020;
    std::string address_grsim = "224.5.23.2";

    UDPServer server(address_grsim, port_vision, io_context);
    while (running) {
        server.send();
        usleep(60);
    };
}

void test_client(boost::asio::io_context& io_context) {
    unsigned int port_command = 20011;
    std::string address_grsim = "224.5.23.2";
    std::string listen_address = "0.0.0.0";

    UDPClient client(listen_address, address_grsim, port_command, io_context);
    while (running) {
        std::cout << "test" << std::endl;
        usleep(60);
    };
}

void update(const boost::system::error_code& /*e*/,
           boost::asio::steady_timer* t) {
    if (running) {

        std::cout << "Hello" << std::endl;

        t->expires_at(t->expiry() + boost::asio::chrono::milliseconds(60));
        t->async_wait(
            boost::bind(update, boost::asio::placeholders::error, t));
    }
}

int main(int argc, char const* argv[]) {
    signal(SIGINT, signalHandler);
    boost::asio::io_context io;

    boost::asio::steady_timer t(io, boost::asio::chrono::milliseconds(60));
    t.async_wait(
        boost::bind(update, boost::asio::placeholders::error, &t));

    io.run();

    return 0;
}
