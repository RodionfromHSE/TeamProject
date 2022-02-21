cmake_minimum_required(VERSION 3.16)
project(MyServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)
# For my own library
#target_include_directories(${PROJECT_NAME}
#        PUBLIC
#            MyLibrary
#        )
#
#target_link_libraries(${PROJECT_NAME}
#        PUBLIC
#            MyLibrary
#        )

# For threads
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
