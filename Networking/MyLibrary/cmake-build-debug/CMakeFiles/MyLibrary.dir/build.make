# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MyLibrary.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MyLibrary.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyLibrary.dir/flags.make

CMakeFiles/MyLibrary.dir/connection.cpp.o: CMakeFiles/MyLibrary.dir/flags.make
CMakeFiles/MyLibrary.dir/connection.cpp.o: ../connection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MyLibrary.dir/connection.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MyLibrary.dir/connection.cpp.o -c /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/connection.cpp

CMakeFiles/MyLibrary.dir/connection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyLibrary.dir/connection.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/connection.cpp > CMakeFiles/MyLibrary.dir/connection.cpp.i

CMakeFiles/MyLibrary.dir/connection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyLibrary.dir/connection.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/connection.cpp -o CMakeFiles/MyLibrary.dir/connection.cpp.s

CMakeFiles/MyLibrary.dir/main.cpp.o: CMakeFiles/MyLibrary.dir/flags.make
CMakeFiles/MyLibrary.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MyLibrary.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MyLibrary.dir/main.cpp.o -c /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/main.cpp

CMakeFiles/MyLibrary.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyLibrary.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/main.cpp > CMakeFiles/MyLibrary.dir/main.cpp.i

CMakeFiles/MyLibrary.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyLibrary.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/main.cpp -o CMakeFiles/MyLibrary.dir/main.cpp.s

CMakeFiles/MyLibrary.dir/server.cpp.o: CMakeFiles/MyLibrary.dir/flags.make
CMakeFiles/MyLibrary.dir/server.cpp.o: ../server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MyLibrary.dir/server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MyLibrary.dir/server.cpp.o -c /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/server.cpp

CMakeFiles/MyLibrary.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyLibrary.dir/server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/server.cpp > CMakeFiles/MyLibrary.dir/server.cpp.i

CMakeFiles/MyLibrary.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyLibrary.dir/server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/server.cpp -o CMakeFiles/MyLibrary.dir/server.cpp.s

# Object files for target MyLibrary
MyLibrary_OBJECTS = \
"CMakeFiles/MyLibrary.dir/connection.cpp.o" \
"CMakeFiles/MyLibrary.dir/main.cpp.o" \
"CMakeFiles/MyLibrary.dir/server.cpp.o"

# External object files for target MyLibrary
MyLibrary_EXTERNAL_OBJECTS =

MyLibrary: CMakeFiles/MyLibrary.dir/connection.cpp.o
MyLibrary: CMakeFiles/MyLibrary.dir/main.cpp.o
MyLibrary: CMakeFiles/MyLibrary.dir/server.cpp.o
MyLibrary: CMakeFiles/MyLibrary.dir/build.make
MyLibrary: CMakeFiles/MyLibrary.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable MyLibrary"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MyLibrary.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MyLibrary.dir/build: MyLibrary

.PHONY : CMakeFiles/MyLibrary.dir/build

CMakeFiles/MyLibrary.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MyLibrary.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MyLibrary.dir/clean

CMakeFiles/MyLibrary.dir/depend:
	cd /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/cmake-build-debug /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/cmake-build-debug /mnt/c/Users/home/Desktop/Programming/C++/TeamProject/Networking/MyLibrary/cmake-build-debug/CMakeFiles/MyLibrary.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MyLibrary.dir/depend
