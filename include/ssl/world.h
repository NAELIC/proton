/*
grSim - RoboCup Small Size Soccer Robots Simulator
Copyright (C) 2011, Parsian Robotic Center (eew.aut.ac.ir/~parsian/grsim)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <queue>
#include <string>

#include "config.h"
#include "constant.h"
#include "grSim_Commands.pb.h"
#include "grSim_Packet.pb.h"
#include "grSim_Replacement.pb.h"
#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_refbox_log.pb.h"
#include "messages_robocup_ssl_robot_status.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"
#include "net/udp_client.h"
#include "net/udp_server.h"
#include "physics/pball.h"
#include "physics/pfixedbox.h"
#include "physics/pground.h"
#include "physics/pworld.h"
#include "robot.h"

#define FORMATION_OUTSIDE 0
#define FORMATION_INSIDE_1 1
#define FORMATION_INSIDE_2 2
#define FORMATION_OUTSIDE_FIELD 3

class RobotsFomation;
class SendingPacket {
   public:
    SendingPacket(SSL_WrapperPacket* _packet, int _t);
    SSL_WrapperPacket* packet;
    int t;
};

class World {
   private:
    int framenum;
    dReal last_dt;
    std::queue<SendingPacket*> sendQueue;
    bool lastInfraredState[NB_TEAM][MAX_ROBOT];
    KickStatus lastKickState[NB_TEAM][MAX_ROBOT];
    int lastStatusSendCount[NB_TEAM][MAX_ROBOT];

    std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>
        frame_time;

   public:
    World(RobotsFomation* form1, RobotsFomation* form2);
    virtual ~World();
    void step(dReal dt = -1);
    SSL_WrapperPacket* generatePacket(int cam_id = 0);
    void addFieldLinesArcs(SSL_GeometryFieldSize* field);
    void addFieldLine(SSL_GeometryFieldSize* field, const std::string& name,
                      float p1_x, float p1_y, float p2_x, float p2_y,
                      float thickness);
    void addFieldArc(SSL_GeometryFieldSize* field, const std::string& name,
                     float c_x, float c_y, float radius, float a1, float a2,
                     float thickness);
    void sendVisionBuffer();
    bool visibleInCam(int id, double x, double y);
    int robotIndex(int robot, int team);
    void addRobotStatus(Robots_Status& robotsPacket, int robotID, bool infrared,
                        KickStatus kickStatus);
    void sendRobotStatus(Robots_Status& robotsPacket, int team);

    PWorld* p;
    PBall* ball;
    PGround* ground;
    PFixedBox* walls[MAX_NB_WALL];
    UDPServer *visionServer, *blueStatusSocket, *yellowStatusSocket;
    UDPClient* commandSocket;
    Robot* robots[MAX_ROBOT * 2];
    int sendGeomCount;

    void recvActions();
};

class RobotsFomation {
   public:
    dReal x[MAX_ROBOT];
    dReal y[MAX_ROBOT];
    RobotsFomation(int type);
    void setAll(dReal* xx, dReal* yy);
};

dReal fric(dReal f);
