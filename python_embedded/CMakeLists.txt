cmake_minimum_required(VERSION 3.0.0)

project(jsontest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -ggdb")

find_package (Python3 COMPONENTS Interpreter Development)
find_package(Boost COMPONENTS python3.7 filesystem REQUIRED)

add_executable(jsontest jsontest.cpp)
target_include_directories(jsontest PUBLIC ${Python3_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(jsontest LINK_PUBLIC ${Python3_LIBRARIES} ${Boost_LIBRARIES})




