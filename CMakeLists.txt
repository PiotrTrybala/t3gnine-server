cmake_minimum_required(VERSION 3.16)
project(tengine-server LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")

find_package(Bullet REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

include_directories(SYSTEM 
    "vendor/glad/include"
    "../tengine/include"
    ${Boost_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} main.cpp core/gameServer.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../lib/tengine/include)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        BulletDynamics
        BulletCollision
        LinearMath
        Boost::boost
        Boost::system
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()