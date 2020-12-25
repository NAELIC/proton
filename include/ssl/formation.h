#pragma once

#include <vector>

#include "utils/math/point.h"

enum Type { A }; // TODO : Add more formations !

class Formation {
   public:
    Formation(Type type);
   private:
    std::vector<Point> positions;
};