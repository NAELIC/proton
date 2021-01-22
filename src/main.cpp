#include <unistd.h>

#include <boost/asio.hpp>
#include <cmath>
#include <csignal>
#include <iostream>
#include <string>

#include "net/udp_client.h"
#include "net/udp_server.h"
#include "proton.h"

boost::asio::io_context io;
bool running = true;

void signalHandler(int signum) {
    running = false;
    io.stop();
    exit(0);
}

int getDesiredFPS() { return (int)floor(1000.0 / getConf().game.desired_fps); }

void update(const boost::system::error_code& /*e*/,
            boost::asio::steady_timer* t, Proton* proton) {
    if (running) {
        proton->tick();
        t->expires_at(t->expiry() +
                      boost::asio::chrono::milliseconds(getDesiredFPS()));
        t->async_wait(
            boost::bind(update, boost::asio::placeholders::error, t, proton));
    }
}

int main(int argc, char const* argv[]) {
    signal(SIGINT, signalHandler);
    auto proton = Proton(io);

    boost::asio::steady_timer t(
        io, boost::asio::chrono::milliseconds(getDesiredFPS()));
    t.async_wait(
        boost::bind(update, boost::asio::placeholders::error, &t, &proton));

    io.run();

    return 0;
}
