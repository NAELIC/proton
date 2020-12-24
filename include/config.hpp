#pragma once

#include <string>

// TODO : Verify types of config
// TODO : Add Division !
// enum Division { A, B };

// TODO : First version, not sure that it's very good !
struct Field {
    float line_width;
    float length;
    float width;
    float radius;
    float free_kick;
    float penalty_width;
    float penalty_height;
    float penalty_point;
    float margin;
    float referee_margin;
    float wall_thickness;
    float goal_width;
    float goal_depth;
    float goal_height;
};

struct Ball {
    float radius = 0.0215;
    float mass = 0.043;
    float friction = 0.05;
    float slip = 1;
    float bounce_factor = 0.5;
    float bounce_velocity = 0.1;
    float linear_dampling = 0.004;
    float angular_dampling = 0.004;
};

struct World {
    int desired_fps = 65;
    float delta_time = 0.016;
    float gravity = 9.8;
    bool reset_turn_over = true;  // TODO : What is it ?
};

struct Communication {
    std::string vision_multicast_adress = "224.5.23.2";
    unsigned int vision_multicast_port = 10020;
    unsigned int command_listen_port = 20011;
    unsigned int blue_status_port = 30011;
    unsigned int yellow_status_port = 30012;
    unsigned int send_delay = 0;
    unsigned int send_geometry = 120;
};

struct Noise {
    bool noise = false;
    double noise_deviation_x = 3;
    double noise_deviation_y = 3;
    double noiseDeviation_angle = 2;

    bool vanishing = false;
    double blue_team_vanishing = 0;
    double yellow_team_vanishing = 0;
    double ball_vanishing = 0;
};

class Config {
   public:
    Field field;  // TODO : Add Division A or B !
};