cmake_minimum_required(VERSION 3.0)

project(bnfc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt REQUIRED)

include_directories(
    include
    rapidxml
    rapidjson/include
)

file(GLOB sources src/*.cpp)
add_executable(bnfc ${sources} main.cpp)
target_link_libraries(bnfc fmt)