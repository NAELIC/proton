# proton simulator

[RoboCup Small Size League](https://ssl.robocup.org/) Simulator.

- License: [GNU General Public License (GPLv3)](LICENSE.md)

It is a [grSim](https://github.com/RoboCup-SSL/grSim) rewriting.
We see that impossible to launch in Docker Windows version without display. Simply we remove the QT dependencies.   
It is only for proof concept for the moment, we plan to reintegrate QT when we finish but in properly way.

## Software Requirements

proton compiles on Linux (tested on Ubuntu and Arch linux variants only).
It depends on the following libraries:
- [CMake](https://cmake.org/) version 3.5+
- [pkg-config](https://freedesktop.org/wiki/Software/pkg-config/)
- [Open Dynamics Engine (ODE)](http://www.ode.org)
- [Google Protobuf](https://github.com/google/protobuf)
- [Boost development libraries](http://www.boost.org/) (needed by VarTypes)

## Usage

Receiving data from grSim is similar to receiving data from [SSL-Vision](https://github.com/RoboCup-SSL/ssl-vision) using [Google Protobuf](https://github.com/google/protobuf) library.
Sending data to the simulator is also possible using Google Protobuf.

## Installation

TODO
