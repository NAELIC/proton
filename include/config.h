#pragma once

#include <string>

// enum Division { A, B };

// TODO : First version, not sure that it's very good !
struct Field {
    double line_width{0.01};
    double length{9.000};
    double width{6.000};
    double radius{0.500};
    double free_kick{0.700};
    double penalty_width{2.00};
    double penalty_depth{1.0};
    double penalty_point{1.0};
    double margin{0.3};
    double referee_margin{0.4};
    double wall_thickness{0.05};
    double goal_thickness{0.02};
    double goal_width{1.0};
    double goal_depth{0.2};
    double goal_height{0.16};
};

struct Ball {
    float radius{0.0215};
    float mass{0.043};
    float friction{0.05};
    float slip{1};
    float bounce_factor{0.5};
    float bounce_velocity{0.1};
    float linear_dampling{0.004};
    float angular_dampling{0.004};
};

struct Communication {
    std::string vision_multicast_adress{"224.5.23.2"};
    unsigned int vision_multicast_port{10020};
    unsigned int command_listen_port{20011};
    unsigned int blue_status_port{30011};
    unsigned int yellow_status_port{30012};
    unsigned int send_delay{0};
    unsigned int send_geometry_every{120};
};

struct Noise {
    bool noise{false};
    double deviation_x{3};
    double deviation_y{3};
    double deviation_angle{2};

    bool vanishing{false};
    double blue_team_vanishing{0};
    double yellow_team_vanishing{0};
    double ball_vanishing{0};
};

struct Game {
    int robot_count{8};
    int desired_fps{60};
    float delta_time{0.016};
    float gravity = {9.8};
    bool reset_turn_over{true};  // TODO : What is it ?
};

// TODO : Add Division A or B !

struct RobotSetting {
    // geometry
    double center_from_kicker{0.064997};
    double radius{0.09};
    double height{0.15};
    double bottom_height{0.02};
    double kicker_z{0.005};
    double kicker_thickness{0.005};
    double kicker_width{0.08};
    double kicker_height{0.04};
    double wheel_radius{0.0285};
    double wheel_thickness{0.005};
    double wheel_angle_1{60};
    double wheel_angle_2{135};
    double wheel_angle_3{225};
    double wheel_angle_4{300};

    // physics
    double body_mass{2.75};
    double wheel_mass{0.06};
    double kicker_mass{0.03125};
    double kicker_damp_factor{0.2};
    double roller_torque_factor{0.06};
    double roller_perpendicular_torque_factor{0.005};
    double kicker_friction{0.8};
    double wheel_tangent_friction{0.8};
    double wheel_perpendicular_friction{0.05};
    double wheel_motor_fMax{0.2};
};

class Config {
   public:
    Field field;
    Ball ball;
    Game game;
    RobotSetting robot_setting;
    Noise noise;
    Communication com;
};

extern Config& getConf();