cmake_minimum_required(VERSION 3.16)
project(MyLibrary)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.71.0 REQUIRED)
find_package(Threads REQUIRED)
#find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# TODO: How to change RUNTIME_OUTPUT_DIR?
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../.)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
#SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
#SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
#SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
#SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
#SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
#SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
message("CMake runtime output Directory : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

link_libraries(Threads::Threads)
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
    else()
        message("All is bad! SFML isn't found!")
    endif()
    if(WIN32)
        # Add sfml's dlls to your cmake build
        file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
        file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
        # TODO: Necessary?
#        file(GLOB MINGW_DEP_DLLS "C:/mingw64/bin/*.dll")
#        file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    endif()
else()
    # Here you are awaited to be on Linux
    include_directories(/usr/local/include)
    find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
    include_directories(${SFML_INCLUDE_DIRS})
    link_libraries(sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
endif ()

add_executable(Server SimpleServer.cpp)

add_executable(Client SimpleClient.cpp)
add_executable(Client2 SimpleClient.cpp)

add_executable(draft DRAFT.cpp)

# Messages
message("Current dir: ${CMAKE_CURRENT_LIST_DIR}")