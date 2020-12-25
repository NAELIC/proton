#include "proton.h"

Proton::Proton() {
    auto r = new RobotsFomation(1);
    world = new World(r,r);
}