cmake_minimum_required(VERSION 3.16)
project(Game)

set(CMAKE_CXX_STANDARD 17)


add_executable(main main.cpp)

include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(main sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
#find_package(Threads REQUIRED)
#target_link_libraries(TeamProject PRIVATE Threads::Threads)
