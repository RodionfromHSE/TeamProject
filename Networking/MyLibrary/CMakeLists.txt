cmake_minimum_required(VERSION 3.16)
project(MyLibrary)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.71.0 REQUIRED)

#include_directories(src)
#file(GLOB SOURCES C:/Users/home/Desktop/garbage/olcPixelGameEngine/Videos/Networking/Parts1&2/SimpleServer.cpp)
add_executable(Server SimpleServer.cpp)
#add_executable(WorkingServer SOURCES)
add_executable(Client SimpleClient.cpp)
add_executable(Client2 SimpleClient.cpp)
add_executable(draft DRAFT.cpp)
target_link_libraries(draft PRIVATE Threads::Threads)


find_package(Threads REQUIRED)
target_link_libraries(Server PRIVATE Threads::Threads)
target_link_libraries(Client PRIVATE Threads::Threads)
target_link_libraries(Client2 PRIVATE Threads::Threads)
