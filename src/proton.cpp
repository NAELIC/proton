#include "proton.h"

Proton::Proton(boost::asio::io_context& io)
    : vision_server(getConf().com.vision_multicast_adress,
                    getConf().com.vision_multicast_port, io),
      command_server("0.0.0.0", getConf().com.vision_multicast_adress,
                     getConf().com.command_listen_port, io) {
    auto r = new RobotsFomation(1);
    world = new World(r, r);
    // unsigned int port_command = 20011;
    //     std::string address_grsim = "224.5.23.2";
    //     std::string listen_address = "0.0.0.0";

    UDPClient client("0.0.0.0", getConf().com.vision_multicast_adress,
                     getConf().com.command_listen_port, io);
    world->visionServer = &vision_server;
    world->commandSocket = &command_server;
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
