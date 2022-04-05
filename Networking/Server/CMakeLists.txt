cmake_minimum_required(VERSION 3.16)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Server main.cpp server.cpp ../Game/object.cpp)

# Messages
message("Current dir: ${CMAKE_CURRENT_LIST_DIR}")
