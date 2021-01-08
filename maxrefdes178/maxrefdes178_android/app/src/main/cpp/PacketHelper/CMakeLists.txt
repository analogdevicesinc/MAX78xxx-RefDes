cmake_minimum_required(VERSION 3.5.0)

# Set root path
set(ROOT_PATH ./../../../../../../)

# Project name
project (maxcamPacketLib)

# Set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

# Set sources for outputs
set(SRC_LIST ${ROOT_PATH}/maxcam/source/MAXCAM_Packet.c ./MAXCAM_PacketHelper.c ${ROOT_PATH}/maxrefdes178_common/maxrefdes178_ble_packet.c)

# Set include headers for lib/application
include_directories(${ROOT_PATH}/include ${ROOT_PATH}/maxcam/include/ ${ROOT_PATH}/maxrefdes178_common/)

# Generate a static library
add_library(maxcamPacketLibStatic STATIC ${SRC_LIST})
