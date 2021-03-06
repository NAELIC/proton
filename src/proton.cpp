#include "proton.h"

Proton::Proton(boost::asio::io_context& io)
    : vision_server(getConf().com.vision_multicast_adress,
                    getConf().com.vision_multicast_port, io),
      command_server("0.0.0.0", getConf().com.vision_multicast_adress,
                     getConf().com.command_listen_port, io),
      blue_status_server(getConf().com.blue_status_port, io),
      yellow_status_server(getConf().com.yellow_status_port, io) {
    auto r = new RobotsFomation(1);
    world = new World(r, r);

    UDPClient client("0.0.0.0", getConf().com.vision_multicast_adress,
                     getConf().com.command_listen_port, io);
    world->visionServer = &vision_server;
    world->commandSocket = &command_server;

    world->blueStatusSocket = &blue_status_server;
    world->yellowStatusSocket = &yellow_status_server;
}

void Proton::tick() {
    if (first) {
        world->step();
        first = false;
        start_time = std::chrono::steady_clock::now();
    } else {
        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> delta_time = current_time - start_time;
        start_time = current_time;
        world->step(delta_time.count());
    }
}

Proton::~Proton() { delete world; }
