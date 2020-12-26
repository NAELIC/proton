#pragma once

#include <boost/asio.hpp>

#include "net/udp_client.h"
#include "net/udp_server.h"
#include "ssl/world.h"
class Proton {
   public:
    World* world;
    UDPServer vision_server;
    UDPClient command_server;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>
        start_time;
    Proton(boost::asio::io_context& io);
    ~Proton();
    void tick();

   private:
    bool first{true};
};