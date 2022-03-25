cmake_minimum_required(VERSION 3.16)
project(MyLibrary)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)
message("CMake runtime output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

find_package(Boost 1.71.0 REQUIRED)
find_package(Threads REQUIRED)

if (WIN32 AND NOT MSVC AND NOT BORLAND)
    # For sockets to work correctly
    link_libraries(wsock32 ws2_32)
    # SFML linking
    include_directories(c:/SFML/include)

    set(SFML_ROOT c:/SFML)
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

    find_package(SFML 2.5 COMPONENTS system window graphics network audio)
    link_libraries(${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

    if (SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
    else ()
        message("All is bad! SFML isn't found!")
    endif ()

    # Add sfml's dlls to your cmake build
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#    file(COPY ${BINARY_DEP_DLLS} DESTINATION C:/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary)
else ()
    # Here you are awaited to be on Linux
    include_directories(/usr/local/include)
    find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
    include_directories(${SFML_INCLUDE_DIRS})
    link_libraries(sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
    # for safe asio usage
    link_libraries(Threads::Threads)
endif ()

add_executable(Server SimpleServer.cpp)

add_executable(Client SimpleClient.cpp)
add_executable(Client2 SimpleClient.cpp)

add_executable(draft DRAFT.cpp)

# Messages
message("Current dir: ${CMAKE_CURRENT_LIST_DIR}")