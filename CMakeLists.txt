cmake_minimum_required(VERSION 3.16)
project(TeamProject)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Networking)
add_subdirectory(Game)
#find_package(Threads REQUIRED)
#target_link_libraries(TeamProject PRIVATE Threads::Threads)
