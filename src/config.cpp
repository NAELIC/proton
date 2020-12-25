#include "config.h"

extern Config& getConf() {
    static auto config = new Config();
    return *config;
}