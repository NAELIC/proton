#include "proton.h"

Proton::Proton() {
    auto r = new RobotsFomation(1);
    world = new World(r, r);
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