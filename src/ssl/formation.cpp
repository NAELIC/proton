#include "ssl/formation.h"

Formation::Formation(Type type) {
    switch (type) {
        case Type::A:
            positions = {
                Point(1.50, 1.12),
                Point(1.50, 0.0),
                Point(1.50, -1.12),
                Point(0.55, 0.0),
                Point(2.50, 0.0),
                Point(3.60, 0.0),
            }; // TODO : Don't like this way !
            break;
        default:
            break;
    }
}