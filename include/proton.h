#pragma once

#include "ssl/world.h"

class Proton {
   public:
    World* world;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>
        start_time;
    Proton();
    ~Proton();
    void tick();

   private:
    bool first{true};
};