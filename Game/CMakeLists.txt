cmake_minimum_required(VERSION 3.18)
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost 1.71.0 REQUIRED)
find_package(Threads REQUIRED)

if (WIN32 AND NOT MSVC AND NOT BORLAND)
    find_package(OpenGL REQUIRED)
#    find_package(GLUT REQUIRED)

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
else ()
    # Here you are awaited to be on Linux
    include_directories(/usr/local/include)
    find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
    include_directories(${SFML_INCLUDE_DIRS})
    link_libraries(sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
    # For safe asio usage
    link_libraries(Threads::Threads)
endif ()

include_directories(../Networking/Library ../Networking/Server ../Networking/Client ../Networking/Game)

set(SOURCE main.cpp box2d.h)
    # network_system.cpp ../Networking/Server/server.cpp ../Networking/Client/client.cpp ../Networking/Game/object.cpp)
#set(SOURCE main.cpp)

#find_package(box2d CONFIG REQUIRED)
add_subdirectory(box2d)

#find_library(BOX2D STATIC NAMES box2d glad glfw imgui sajson PATHS lib REQUIRED)
#message("-- Box2d found in " ${BOX2D} ${CACHE})

add_executable(main ${SOURCE})
target_link_libraries(main LINK_PUBLIC box2d)
target_include_directories(main PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/box2d")
message("-- Files included from " "${CMAKE_CURRENT_LIST_DIR}/include/box2d")
#target_link_libraries(main PRIVATE Box2D::Box2D)
#target_link_libraries(main LINK_PUBLIC ${OPENGL_LIBRARIES})
