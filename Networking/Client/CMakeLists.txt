cmake_minimum_required(VERSION 3.16)
project(Client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Client main.cpp client.cpp)
#add_executable(draft DRAFT.cpp)

# Messages
#message("Current dir: ${CMAKE_CURRENT_LIST_DIR}")
