cmake_minimum_required(VERSION 3.16)
project(tengine-server LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Boost ---
set(BOOST_INCLUDE_LIBRARIES
    thread
    filesystem
    system
    program_options
)
set(BOOST_ENABLE_CMAKE ON)
set(BUILD_TESTING OFF)

include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.82.0
)
FetchContent_MakeAvailable(Boost)

# --- Bullet ---
find_package(Bullet REQUIRED)

# --- Executable ---
add_executable(${PROJECT_NAME} main.cpp)

# --- Link ---
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        BulletDynamics
        BulletCollision
        LinearMath
        Boost::asio
        Boost::filesystem
        Boost::thread
        Boost::program_options
)

# --- Warnings ---
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)
