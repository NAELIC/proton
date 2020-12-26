#include "proton.h"

Proton::Proton(boost::asio::io_context& io)
    : vision_server(getConf().com.vision_multicast_adress,
                    getConf().com.vision_multicast_port, io) {
    auto r = new RobotsFomation(1);
    world = new World(r, r);
    world->visionServer = &vision_server;
}

void Proton::tick() {
    if (first) {
        world->step();
        first = false;
        start_time = std::chrono::steady_clock::now();
    } else {
        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> delta_time =
            std::chrono::steady_clock::now() - start_time;
        start_time = current_time;
        world->step(delta_time.count());
    }
}

Proton::~Proton() { delete world; }
