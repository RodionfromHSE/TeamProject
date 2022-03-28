cmake_minimum_required(VERSION 3.16)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
##message("CMake runtime output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
#
#find_package(Boost 1.71.0 REQUIRED)
#find_package(Threads REQUIRED)
#
#if (WIN32 AND NOT MSVC AND NOT BORLAND)
#    # For sockets to work correctly
#    link_libraries(wsock32 ws2_32)
#else ()
#    link_libraries(Threads::Threads)
#endif ()

add_executable(main main.cpp)

#add_executable(draft DRAFT.cpp)

# Messages
message("Current dir: ${CMAKE_CURRENT_LIST_DIR}")
