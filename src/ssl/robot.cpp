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

#include "ssl/robot.h"

// ang2 = position angle
// ang  = rotation angle
Robot::Wheel::Wheel(Robot* robot, int _id, dReal ang, dReal ang2) {
    id = _id;
    rob = robot;
    dReal rad = getConf().robot_setting.radius -
                getConf().robot_setting.wheel_thickness / 2.0;
    ang *= M_PI / 180.0f;
    ang2 *= M_PI / 180.0f;
    dReal x = rob->m_x;
    dReal y = rob->m_y;
    dReal z = rob->m_z;
    dReal centerx = x + rad * cos(ang2);
    dReal centery = y + rad * sin(ang2);
    dReal centerz = z - getConf().robot_setting.height * 0.5 +
                    getConf().robot_setting.wheel_radius -
                    getConf().robot_setting.bottom_height;
    cyl = new PCylinder(centerx, centery, centerz,
                        getConf().robot_setting.wheel_radius,
                        getConf().robot_setting.wheel_thickness,
                        getConf().robot_setting.wheel_mass);
    cyl->setRotation(-sin(ang), cos(ang), 0, M_PI * 0.5);
    cyl->setBodyRotation(-sin(ang), cos(ang), 0, M_PI * 0.5,
                         true);  // set local rotation matrix
    cyl->setBodyPosition(centerx - x, centery - y, centerz - z,
                         true);  // set local position vector
    cyl->space = rob->space;

    rob->w->addObject(cyl);

    joint = dJointCreateHinge(rob->w->world, 0);

    dJointAttach(joint, rob->chassis->body, cyl->body);
    const dReal* a = dBodyGetPosition(cyl->body);
    dJointSetHingeAxis(joint, cos(ang), sin(ang), 0);
    dJointSetHingeAnchor(joint, a[0], a[1], a[2]);

    motor = dJointCreateAMotor(rob->w->world, 0);
    dJointAttach(motor, rob->chassis->body, cyl->body);
    dJointSetAMotorNumAxes(motor, 1);
    dJointSetAMotorAxis(motor, 0, 1, cos(ang), sin(ang), 0);
    dJointSetAMotorParam(motor, dParamFMax,
                         getConf().robot_setting.wheel_motor_fMax);
    speed = 0;
}

void Robot::Wheel::step() {
    dJointSetAMotorParam(motor, dParamVel, speed);
    dJointSetAMotorParam(motor, dParamFMax,
                         getConf().robot_setting.wheel_motor_fMax);
}

Robot::Kicker::Kicker(Robot* robot) : holdingBall(false) {
    rob = robot;

    dReal x = rob->m_x;
    dReal y = rob->m_y;
    dReal z = rob->m_z;
    dReal centerx = x + (getConf().robot_setting.center_from_kicker +
                         getConf().robot_setting.kicker_thickness);
    dReal centery = y;
    dReal centerz = z - (getConf().robot_setting.height) * 0.5f +
                    getConf().robot_setting.wheel_radius -
                    getConf().robot_setting.bottom_height +
                    getConf().robot_setting.kicker_z;
    box = new PBox(centerx, centery, centerz,
                   getConf().robot_setting.kicker_thickness,
                   getConf().robot_setting.kicker_width,
                   getConf().robot_setting.kicker_height,
                   getConf().robot_setting.kicker_mass);
    box->setBodyPosition(centerx - x, centery - y, centerz - z, true);
    box->space = rob->space;

    rob->w->addObject(box);

    joint = dJointCreateHinge(rob->w->world, 0);
    dJointAttach(joint, rob->chassis->body, box->body);
    const dReal* aa = dBodyGetPosition(box->body);
    dJointSetHingeAnchor(joint, aa[0], aa[1], aa[2]);
    dJointSetHingeAxis(joint, 0, -1, 0);

    dJointSetHingeParam(joint, dParamVel, 0);
    dJointSetHingeParam(joint, dParamLoStop, 0);
    dJointSetHingeParam(joint, dParamHiStop, 0);

    rolling = 0;
    kicking = NO_KICK;
}

void Robot::Kicker::step() {
    if (!isTouchingBall() || rolling == 0) unholdBall();
    if (kicking != NO_KICK) {
        kickstate--;
        if (kickstate <= 0) kicking = NO_KICK;
    } else if (rolling != 0) {
        if (isTouchingBall()) {
            holdBall();
        }
    }
}

bool Robot::Kicker::isTouchingBall() {
    dReal vx, vy, vz;
    dReal bx, by, bz;
    dReal kx, ky, kz;
    rob->chassis->getBodyDirection(vx, vy, vz);
    rob->getBall()->getBodyPosition(bx, by, bz);
    box->getBodyPosition(kx, ky, kz);
    kx += vx * getConf().robot_setting.kicker_thickness * 0.5f;
    ky += vy * getConf().robot_setting.kicker_thickness * 0.5f;
    dReal xx = fabs((kx - bx) * vx + (ky - by) * vy);
    dReal yy = fabs(-(kx - bx) * vy + (ky - by) * vx);
    dReal zz = fabs(kz - bz);
    return ((xx < getConf().robot_setting.kicker_thickness * 2.0f +
                      getConf().ball.radius) &&
            (yy < getConf().robot_setting.kicker_width * 0.5f) &&
            (zz < getConf().robot_setting.kicker_height * 0.5f));
}

KickStatus Robot::Kicker::isKicking() { return kicking; }

void Robot::Kicker::setRoller(int roller) { rolling = roller; }

int Robot::Kicker::getRoller() { return rolling; }

void Robot::Kicker::toggleRoller() {
    if (rolling == 0)
        rolling = 1;
    else
        rolling = 0;
}

void Robot::Kicker::kick(dReal kickspeedx, dReal kickspeedz) {
    dReal dx, dy, dz;
    dReal vx, vy, vz;
    rob->chassis->getBodyDirection(dx, dy, dz);
    dz = 0;
    dReal zf = kickspeedz;
    unholdBall();
    if (isTouchingBall()) {
        dReal dlen = dx * dx + dy * dy + dz * dz;
        dlen = sqrt(dlen);
        vx = dx * kickspeedx / dlen;
        vy = dy * kickspeedx / dlen;
        vz = zf;
        const dReal* vball = dBodyGetLinearVel(rob->getBall()->body);
        dReal vn = -(vball[0] * dx + vball[1] * dy) *
                   getConf().robot_setting.kicker_damp_factor;
        dReal vt = -(vball[0] * dy - vball[1] * dx);
        vx += vn * dx - vt * dy;
        vy += vn * dy + vt * dx;
        dBodySetLinearVel(rob->getBall()->body, vx, vy, vz);
        if (kickspeedz >= 1)
            kicking = CHIP_KICK;
        else
            kicking = FLAT_KICK;
        kickstate = 10;
    }
}

void Robot::Kicker::holdBall() {
    dReal vx, vy, vz;
    dReal bx, by, bz;
    dReal kx, ky, kz;
    rob->chassis->getBodyDirection(vx, vy, vz);
    rob->getBall()->getBodyPosition(bx, by, bz);
    box->getBodyPosition(kx, ky, kz);
    kx += vx * getConf().robot_setting.kicker_thickness * 0.5f;
    ky += vy * getConf().robot_setting.kicker_thickness * 0.5f;
    dReal xx = fabs((kx - bx) * vx + (ky - by) * vy);
    if (holdingBall || xx - getConf().ball.radius < 0) return;
    dBodySetLinearVel(rob->getBall()->body, 0, 0, 0);
    robot_to_ball = dJointCreateHinge(rob->getWorld()->world, 0);
    dJointAttach(robot_to_ball, box->body, rob->getBall()->body);
    holdingBall = true;
}

void Robot::Kicker::unholdBall() {
    if (holdingBall) {
        dJointDestroy(robot_to_ball);
        holdingBall = false;
    }
}

Robot::Robot(PWorld* world, PBall* ball, dReal x, dReal y, dReal z, int rob_id,
             int dir) {
    m_x = x;
    m_y = y;
    m_z = z;
    w = world;
    m_ball = ball;
    m_dir = dir;
    m_rob_id = rob_id;

    space = w->space;

    chassis = new PCylinder(x, y, z, getConf().robot_setting.radius,
                            getConf().robot_setting.height,
                            getConf().robot_setting.body_mass * 0.99f, true);
    chassis->space = space;
    w->addObject(chassis);

    dummy = new PBall(x, y, z, getConf().robot_setting.center_from_kicker,
                      getConf().robot_setting.body_mass * 0.01f);

    dummy->space = space;
    w->addObject(dummy);

    dummy_to_chassis = dJointCreateFixed(world->world, 0);
    dJointAttach(dummy_to_chassis, chassis->body, dummy->body);

    kicker = new Kicker(this);

    wheels[0] = new Wheel(this, 0, getConf().robot_setting.wheel_angle_1,
                          getConf().robot_setting.wheel_angle_1);
    wheels[1] = new Wheel(this, 1, getConf().robot_setting.wheel_angle_2,
                          getConf().robot_setting.wheel_angle_2);
    wheels[2] = new Wheel(this, 2, getConf().robot_setting.wheel_angle_3,
                          getConf().robot_setting.wheel_angle_3);
    wheels[3] = new Wheel(this, 3, getConf().robot_setting.wheel_angle_4,
                          getConf().robot_setting.wheel_angle_4);
    firsttime = true;
    on = true;
}

Robot::~Robot() {}

PBall* Robot::getBall() { return m_ball; }

PWorld* Robot::getWorld() { return w; }

int Robot::getID() { return m_rob_id - 1; }

void Robot::step() {
    if (on) {
        if (firsttime) {
            if (m_dir == -1) setDir(180);
            firsttime = false;
        }
        wheels[0]->step();
        wheels[1]->step();
        wheels[2]->step();
        wheels[3]->step();
        kicker->step();
    } else {
        if (last_state) {
            this->resetSpeeds();
            kicker->setRoller(0);
            wheels[0]->step();
            wheels[1]->step();
            wheels[2]->step();
            wheels[3]->step();
            kicker->step();
        }
    }
    last_state = on;
}

void Robot::resetSpeeds() {
    wheels[0]->speed = wheels[1]->speed = wheels[2]->speed = wheels[3]->speed =
        0;
}

void Robot::resetRobot() {
    resetSpeeds();
    dBodySetLinearVel(chassis->body, 0, 0, 0);
    dBodySetAngularVel(chassis->body, 0, 0, 0);
    dBodySetLinearVel(dummy->body, 0, 0, 0);
    dBodySetAngularVel(dummy->body, 0, 0, 0);
    dBodySetLinearVel(kicker->box->body, 0, 0, 0);
    dBodySetAngularVel(kicker->box->body, 0, 0, 0);
    for (int i = 0; i < 4; i++) {
        dBodySetLinearVel(wheels[i]->cyl->body, 0, 0, 0);
        dBodySetAngularVel(wheels[i]->cyl->body, 0, 0, 0);
    }
    dReal x, y;
    getXY(x, y);
    setXY(x, y);
    if (m_dir == -1)
        setDir(180);
    else
        setDir(0);
}

void Robot::getXY(dReal& x, dReal& y) {
    dReal xx, yy, zz;
    chassis->getBodyPosition(xx, yy, zz);
    x = xx;
    y = yy;
}

dReal Robot::getDir() {
    dReal x, y, z;
    chassis->getBodyDirection(x, y, z);
    dReal dot = x;  // zarb dar (1.0,0.0,0.0)
    dReal length = sqrt(x * x + y * y);
    dReal absAng = (dReal)(acos((dReal)(dot / length)) * (180.0f / M_PI));
    return (y > 0) ? absAng : -absAng;
}

dReal Robot::getDir(dReal& k) {
    dReal x, y, z;
    chassis->getBodyDirection(x, y, z, k);
    dReal dot = x;  // zarb dar (1.0,0.0,0.0)
    dReal length = sqrt(x * x + y * y);
    dReal absAng = (dReal)(acos((dReal)(dot / length)) * (180.0f / M_PI));
    return (y > 0) ? absAng : -absAng;
}

void Robot::setXY(dReal x, dReal y) {
    dReal xx, yy, zz, kx, ky, kz;
    dReal height = ROBOT_START_Z();
    chassis->getBodyPosition(xx, yy, zz);
    chassis->setBodyPosition(x, y, height);
    dummy->setBodyPosition(x, y, height);
    kicker->box->getBodyPosition(kx, ky, kz);
    kicker->box->setBodyPosition(kx - xx + x, ky - yy + y, kz - zz + height);
    for (int i = 0; i < 4; i++) {
        wheels[i]->cyl->getBodyPosition(kx, ky, kz);
        wheels[i]->cyl->setBodyPosition(kx - xx + x, ky - yy + y,
                                        kz - zz + height);
    }
}

void Robot::setDir(dReal ang) {
    ang *= M_PI / 180.0f;
    chassis->setBodyRotation(0, 0, 1, ang);
    kicker->box->setBodyRotation(0, 0, 1, ang);
    dummy->setBodyRotation(0, 0, 1, ang);
    dMatrix3 wLocalRot, wRot, cRot;
    dVector3 localPos, finalPos, cPos;
    chassis->getBodyPosition(cPos[0], cPos[1], cPos[2], false);
    chassis->getBodyRotation(cRot, false);
    kicker->box->getBodyPosition(localPos[0], localPos[1], localPos[2], true);
    dMultiply0(finalPos, cRot, localPos, 4, 3, 1);
    finalPos[0] += cPos[0];
    finalPos[1] += cPos[1];
    finalPos[2] += cPos[2];
    kicker->box->setBodyPosition(finalPos[0], finalPos[1], finalPos[2], false);
    for (int i = 0; i < 4; i++) {
        wheels[i]->cyl->getBodyRotation(wLocalRot, true);
        dMultiply0(wRot, cRot, wLocalRot, 3, 3, 3);
        dBodySetRotation(wheels[i]->cyl->body, wRot);
        wheels[i]->cyl->getBodyPosition(localPos[0], localPos[1], localPos[2],
                                        true);
        dMultiply0(finalPos, cRot, localPos, 4, 3, 1);
        finalPos[0] += cPos[0];
        finalPos[1] += cPos[1];
        finalPos[2] += cPos[2];
        wheels[i]->cyl->setBodyPosition(finalPos[0], finalPos[1], finalPos[2],
                                        false);
    }
}

void Robot::setSpeed(int i, dReal s) {
    if (!((i >= 4) || (i < 0))) wheels[i]->speed = s;
}

void Robot::setSpeed(dReal vx, dReal vy, dReal vw) {
    // Calculate Motor Speeds
    dReal _DEG2RAD = M_PI / 180.0;
    dReal motorAlpha[4] = {Config().robot_setting.wheel_angle_1 * _DEG2RAD,
                           Config().robot_setting.wheel_angle_2 * _DEG2RAD,
                           Config().robot_setting.wheel_angle_3 * _DEG2RAD,
                           Config().robot_setting.wheel_angle_4 * _DEG2RAD};

    dReal dw1 = (1.0 / Config().robot_setting.wheel_radius) *
                (((Config().robot_setting.radius * vw) -
                  (vx * sin(motorAlpha[0])) + (vy * cos(motorAlpha[0]))));
    dReal dw2 = (1.0 / Config().robot_setting.wheel_radius) *
                (((Config().robot_setting.radius * vw) -
                  (vx * sin(motorAlpha[1])) + (vy * cos(motorAlpha[1]))));
    dReal dw3 = (1.0 / Config().robot_setting.wheel_radius) *
                (((Config().robot_setting.radius * vw) -
                  (vx * sin(motorAlpha[2])) + (vy * cos(motorAlpha[2]))));
    dReal dw4 = (1.0 / Config().robot_setting.wheel_radius) *
                (((Config().robot_setting.radius * vw) -
                  (vx * sin(motorAlpha[3])) + (vy * cos(motorAlpha[3]))));

    setSpeed(0, dw1);
    setSpeed(1, dw2);
    setSpeed(2, dw3);
    setSpeed(3, dw4);
}

dReal Robot::getSpeed(int i) {
    if ((i >= 4) || (i < 0)) return -1;
    return wheels[i]->speed;
}
