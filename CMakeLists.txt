cmake_minimum_required(VERSION 3.15)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(Maze-generator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.0
)

set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(BUILD_GAMES    OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(raylib)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

#find_package(raylib REQUIRED)

add_executable(Maze-generator ${SOURCES} ${HEADERS})

target_link_libraries(Maze-generator PRIVATE raylib)
target_include_directories(Maze-generator PRIVATE include)
