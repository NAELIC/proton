# proton simulator

[RoboCup Small Size League](https://ssl.robocup.org/) Simulator.

- License: [GNU General Public License (GPLv3)](LICENSE.md)

It is a [grSim](https://github.com/RoboCup-SSL/grSim) rewriting.
We want to launch in Docker for Windows and server without displaying images.
We remove QT and VarTypes dependencies.

The development is focused to clean the code and reintegrate some features that we left (configuration files, ...).
After that, we plan to export this project has a library and integrate in a new QT project.

## Software Requirements

proton compiles on Linux (tested on Ubuntu and Arch linux variants only).
It depends on the following libraries:
- [CMake](https://cmake.org/) version 3.16.2+
- [Open Dynamics Engine (ODE)](http://www.ode.org)
- [Google Protobuf](https://github.com/google/protobuf)
- [Boost development libraries](http://www.boost.org/)

## Usage

Receiving data from grSim is similar to receiving data from [SSL-Vision](https://github.com/RoboCup-SSL/ssl-vision) using [Google Protobuf](https://github.com/google/protobuf) library.
Sending data to the simulator is also possible using Google Protobuf.

## Installation and Building

WIP (The install process will be given in the future bookstack of the team !)

## Contribute

If you want to contribute on the project, please feel free to contact the team leader of NAELIC [contact@etienne-schmitz.com](mailto:contact@etienne-schmitz.com).
We add issue, PR template.
