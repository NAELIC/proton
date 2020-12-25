#include <unistd.h>

#include <csignal>
#include <iostream>
#include <string>

#include "udp_client.h"
#include "udp_server.h"

bool running = true;

void signalHandler(int signum) {
    running = false;
    exit(1);
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

int main(int argc, char const* argv[]) {
    signal(SIGINT, signalHandler);
    boost::asio::io_context io_context;

    // boost::asio::io_context io_context;
    // UDPServer server(io_context,
    // boost::asio::ip::make_address("224.5.23.2")); io_context.run();

    // boost::asio::io_service io_service;
    // UDPClient r(io_service, boost::asio::ip::address::from_string("0.0.0.0"),
    //             boost::asio::ip::address::from_string("224.5.23.2"));

    // io_service.run();
    // test_server(io_context);
    test_client(io_context);
    // while (running) {
    //     server.send();
    //     usleep(60);
    // };
    std::cout << "Finish !" << std::endl;

    return 0;
}
