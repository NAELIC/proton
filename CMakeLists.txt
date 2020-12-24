cmake_minimum_required(VERSION 3.16.2)
project(proton)

# Settings
set(VERSION "0.0.1a1")
set(MAINTAINER "Etienne SCHMITZ <hello@etienne-schmitz.com>")
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_SOURCE_DIR STREQUAL namec_core_SOURCE_DIR)
    include(GNUInstallDirs)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

    include(cmake/build_configuration.cmake)
    setDefaultBuildType("Debug")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR/}include/physics)

# ODE
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package(ODE REQUIRED)

# Google Protobuf
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
    src/proto/messages_robocup_ssl_detection.proto
    src/proto/messages_robocup_ssl_geometry.proto
    src/proto/messages_robocup_ssl_wrapper.proto
    src/proto/messages_robocup_ssl_refbox_log.proto
    src/proto/messages_robocup_ssl_robot_status.proto
    src/proto/grSim_Replacement.proto
    src/proto/grSim_Commands.proto
    src/proto/grSim_Packet.proto
)

# Sources & Headers
SET(
    SRCS
    src/main.cpp

    # physics
    src/physics/pworld.cpp
    src/physics/pobject.cpp
    src/physics/pball.cpp
    src/physics/pground.cpp
    src/physics/pfixedbox.cpp
    src/physics/pcylinder.cpp
    src/physics/pbox.cpp
    src/physics/pray.cpp
)

SET(
    HEADERS

    # physics
    include/physics/pworld.h
    include/physics/pobject.h
    include/physics/pball.h
    include/physics/pground.h
    include/physics/pfixedbox.h
    include/physics/pcylinder.h
    include/physics/pbox.h
    include/physics/pray.h
)

# executable
add_executable(proton ${SRCS} ${HEADERS} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(proton ode::ode ${PROTOBUF_LIBRARIES})