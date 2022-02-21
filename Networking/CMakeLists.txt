cmake_minimum_required(VERSION 3.16)
project(Networking)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(MyLibrary)
add_subdirectory(MyClient)
add_subdirectory(MyServer)
#find_package(Threads REQUIRED)
#target_link_libraries(TeamProject PRIVATE Threads::Threads)
