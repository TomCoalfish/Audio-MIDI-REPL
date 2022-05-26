# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/quake/Projects/audio-analysis/src/pffft

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/quake/Projects/audio-analysis/src/pffft/build

# Include any dependencies generated for this target.
include CMakeFiles/test_pffft_double.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_pffft_double.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_pffft_double.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_pffft_double.dir/flags.make

CMakeFiles/test_pffft_double.dir/test_pffft.c.o: CMakeFiles/test_pffft_double.dir/flags.make
CMakeFiles/test_pffft_double.dir/test_pffft.c.o: /home/quake/Projects/audio-analysis/src/pffft/test_pffft.c
CMakeFiles/test_pffft_double.dir/test_pffft.c.o: CMakeFiles/test_pffft_double.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/quake/Projects/audio-analysis/src/pffft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test_pffft_double.dir/test_pffft.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_pffft_double.dir/test_pffft.c.o -MF CMakeFiles/test_pffft_double.dir/test_pffft.c.o.d -o CMakeFiles/test_pffft_double.dir/test_pffft.c.o -c /home/quake/Projects/audio-analysis/src/pffft/test_pffft.c

CMakeFiles/test_pffft_double.dir/test_pffft.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_pffft_double.dir/test_pffft.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/quake/Projects/audio-analysis/src/pffft/test_pffft.c > CMakeFiles/test_pffft_double.dir/test_pffft.c.i

CMakeFiles/test_pffft_double.dir/test_pffft.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_pffft_double.dir/test_pffft.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/quake/Projects/audio-analysis/src/pffft/test_pffft.c -o CMakeFiles/test_pffft_double.dir/test_pffft.c.s

# Object files for target test_pffft_double
test_pffft_double_OBJECTS = \
"CMakeFiles/test_pffft_double.dir/test_pffft.c.o"

# External object files for target test_pffft_double
test_pffft_double_EXTERNAL_OBJECTS =

test_pffft_double: CMakeFiles/test_pffft_double.dir/test_pffft.c.o
test_pffft_double: CMakeFiles/test_pffft_double.dir/build.make
test_pffft_double: libpffft.a
test_pffft_double: CMakeFiles/test_pffft_double.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/quake/Projects/audio-analysis/src/pffft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_pffft_double"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_pffft_double.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_pffft_double.dir/build: test_pffft_double
.PHONY : CMakeFiles/test_pffft_double.dir/build

CMakeFiles/test_pffft_double.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_pffft_double.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_pffft_double.dir/clean

CMakeFiles/test_pffft_double.dir/depend:
	cd /home/quake/Projects/audio-analysis/src/pffft/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/quake/Projects/audio-analysis/src/pffft /home/quake/Projects/audio-analysis/src/pffft /home/quake/Projects/audio-analysis/src/pffft/build /home/quake/Projects/audio-analysis/src/pffft/build /home/quake/Projects/audio-analysis/src/pffft/build/CMakeFiles/test_pffft_double.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_pffft_double.dir/depend
