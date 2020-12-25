#include <csignal>
#include <iostream>
#include <string>

#include "udp_server.h"

bool running = true;

void signalHandler(int signum) { running = false; }

int main(int argc, char const *argv[]) {
    signal(SIGINT, signalHandler);

    std::cout << "Hello world !" << std::endl;
    boost::asio::io_context io_context;
    UDPServer server(io_context, boost::asio::ip::make_address("224.5.23.2"));
    io_context.run();

    while (running) server.send();
    std::cout << "Finish !" << std::endl;

    return 0;
}
