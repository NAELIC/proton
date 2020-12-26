#pragma once

#include "ssl/world.h"
#include "net/udp_server.h"
#include <boost/asio.hpp>

class Proton {
   public:
    World* world;
    UDPServer vision_server;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>
        start_time;
    Proton(boost::asio::io_context& io);
    ~Proton();
    void tick();

   private:
    bool first{true};
};