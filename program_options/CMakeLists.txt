cmake_minimum_required(VERSION 3.0.0)

project(program_options)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

find_package(Boost COMPONENTS program_options REQUIRED)
add_executable(program_options main.cpp options.cpp)
TARGET_LINK_LIBRARIES(program_options LINK_PUBLIC ${Boost_LIBRARIES})