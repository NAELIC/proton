#include <csignal>
#include <iostream>
#include <string>

#include "udp_server.h"
#include "udp_client.h"
#include <unistd.h>

bool running = true;

void signalHandler(int signum) { running = false; exit(1); }

int main(int argc, char const *argv[]) {
    signal(SIGINT, signalHandler);

    unsigned int port_command = 20011, port_vision = 10020;
    std::string adress_vision = "224.5.23.2";

    boost::asio::io_context io_context;
    UDPServer server(io_context, boost::asio::ip::make_address("224.5.23.2"));
    io_context.run();

    // boost::asio::io_service io_service;
    // UDPClient r(io_service, boost::asio::ip::address::from_string("0.0.0.0"),
    //             boost::asio::ip::address::from_string("224.5.23.2"));

    // io_service.run();

    while (running) {
        server.send();
        usleep(60);
    };
    std::cout << "Finish !" << std::endl;

    return 0;
}
