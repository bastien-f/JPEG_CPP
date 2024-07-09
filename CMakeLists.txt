cmake_minimum_required(VERSION 3.8)
project(jpeg_compression)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(jpeg_compression ${SOURCES})