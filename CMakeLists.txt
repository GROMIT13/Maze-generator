cmake_minimum_required(VERSION 3.15)
project(RaylibProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

find_package(raylib REQUIRED)

add_executable(labirynth ${SOURCES} ${HEADERS})

target_link_libraries(labirynth PRIVATE raylib)
target_include_directories(labirynth PRIVATE include)