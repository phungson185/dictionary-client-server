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
CMAKE_SOURCE_DIR = /home/sonphung/dictionary-client-server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sonphung/dictionary-client-server/build

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/src/client.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/client.c.o: ../src/client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sonphung/dictionary-client-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/client.dir/src/client.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/client.c.o   -c /home/sonphung/dictionary-client-server/src/client.c

CMakeFiles/client.dir/src/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/client.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sonphung/dictionary-client-server/src/client.c > CMakeFiles/client.dir/src/client.c.i

CMakeFiles/client.dir/src/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/client.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sonphung/dictionary-client-server/src/client.c -o CMakeFiles/client.dir/src/client.c.s

CMakeFiles/client.dir/src/dict.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/dict.c.o: ../src/dict.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sonphung/dictionary-client-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/client.dir/src/dict.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/dict.c.o   -c /home/sonphung/dictionary-client-server/src/dict.c

CMakeFiles/client.dir/src/dict.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/dict.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sonphung/dictionary-client-server/src/dict.c > CMakeFiles/client.dir/src/dict.c.i

CMakeFiles/client.dir/src/dict.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/dict.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sonphung/dictionary-client-server/src/dict.c -o CMakeFiles/client.dir/src/dict.c.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/src/client.c.o" \
"CMakeFiles/client.dir/src/dict.c.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client: CMakeFiles/client.dir/src/client.c.o
client: CMakeFiles/client.dir/src/dict.c.o
client: CMakeFiles/client.dir/build.make
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sonphung/dictionary-client-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client

.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd /home/sonphung/dictionary-client-server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sonphung/dictionary-client-server /home/sonphung/dictionary-client-server /home/sonphung/dictionary-client-server/build /home/sonphung/dictionary-client-server/build /home/sonphung/dictionary-client-server/build/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

