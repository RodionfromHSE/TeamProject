cmake_minimum_required(VERSION 3.16)
project(MyLibrary)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.71.0 REQUIRED)

#include_directories(src)
file(GLOB SOURCES *.cpp *.h)
add_executable(${PROJECT_NAME} ${SOURCES})


find_package(Threads REQUIRED)
target_link_libraries(MyLibrary PRIVATE Threads::Threads)