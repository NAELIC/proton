#pragma once

#include "physics/pworld.h"
#include "physics/pball.h"
#include "physics/pground.h"
#include "physics/pray.h"
#include "physics/pfixedbox.h"
#include "config.h"
#include "constant.h"

class World {
   public:
    PWorld* pworld;
    PBall* pball;
    PGround* pground;
    PRay* pray;
    PFixedBox* walls[MAX_NB_WALL];

    World();
};