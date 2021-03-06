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
CMAKE_SOURCE_DIR = /home/quake/Projects/fluidsynth/fluidsynth/src/gentables

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/quake/Projects/fluidsynth/fluidsynth/build/src/gentables

# Include any dependencies generated for this target.
include CMakeFiles/make_tables.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/make_tables.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/make_tables.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/make_tables.dir/flags.make

CMakeFiles/make_tables.dir/make_tables.c.o: CMakeFiles/make_tables.dir/flags.make
CMakeFiles/make_tables.dir/make_tables.c.o: /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/make_tables.c
CMakeFiles/make_tables.dir/make_tables.c.o: CMakeFiles/make_tables.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/quake/Projects/fluidsynth/fluidsynth/build/src/gentables/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/make_tables.dir/make_tables.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/make_tables.dir/make_tables.c.o -MF CMakeFiles/make_tables.dir/make_tables.c.o.d -o CMakeFiles/make_tables.dir/make_tables.c.o -c /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/make_tables.c

CMakeFiles/make_tables.dir/make_tables.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/make_tables.dir/make_tables.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/make_tables.c > CMakeFiles/make_tables.dir/make_tables.c.i

CMakeFiles/make_tables.dir/make_tables.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/make_tables.dir/make_tables.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/make_tables.c -o CMakeFiles/make_tables.dir/make_tables.c.s

CMakeFiles/make_tables.dir/gen_conv.c.o: CMakeFiles/make_tables.dir/flags.make
CMakeFiles/make_tables.dir/gen_conv.c.o: /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/gen_conv.c
CMakeFiles/make_tables.dir/gen_conv.c.o: CMakeFiles/make_tables.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/quake/Projects/fluidsynth/fluidsynth/build/src/gentables/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/make_tables.dir/gen_conv.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/make_tables.dir/gen_conv.c.o -MF CMakeFiles/make_tables.dir/gen_conv.c.o.d -o CMakeFiles/make_tables.dir/gen_conv.c.o -c /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/gen_conv.c

CMakeFiles/make_tables.dir/gen_conv.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/make_tables.dir/gen_conv.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/gen_conv.c > CMakeFiles/make_tables.dir/gen_conv.c.i

CMakeFiles/make_tables.dir/gen_conv.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/make_tables.dir/gen_conv.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/gen_conv.c -o CMakeFiles/make_tables.dir/gen_conv.c.s

CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.o: CMakeFiles/make_tables.dir/flags.make
CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.o: /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/gen_rvoice_dsp.c
CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.o: CMakeFiles/make_tables.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/quake/Projects/fluidsynth/fluidsynth/build/src/gentables/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.o -MF CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.o.d -o CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.o -c /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/gen_rvoice_dsp.c

CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/gen_rvoice_dsp.c > CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.i

CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/quake/Projects/fluidsynth/fluidsynth/src/gentables/gen_rvoice_dsp.c -o CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.s

# Object files for target make_tables
make_tables_OBJECTS = \
"CMakeFiles/make_tables.dir/make_tables.c.o" \
"CMakeFiles/make_tables.dir/gen_conv.c.o" \
"CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.o"

# External object files for target make_tables
make_tables_EXTERNAL_OBJECTS =

make_tables.exe: CMakeFiles/make_tables.dir/make_tables.c.o
make_tables.exe: CMakeFiles/make_tables.dir/gen_conv.c.o
make_tables.exe: CMakeFiles/make_tables.dir/gen_rvoice_dsp.c.o
make_tables.exe: CMakeFiles/make_tables.dir/build.make
make_tables.exe: CMakeFiles/make_tables.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/quake/Projects/fluidsynth/fluidsynth/build/src/gentables/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable make_tables.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/make_tables.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/make_tables.dir/build: make_tables.exe
.PHONY : CMakeFiles/make_tables.dir/build

CMakeFiles/make_tables.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/make_tables.dir/cmake_clean.cmake
.PHONY : CMakeFiles/make_tables.dir/clean

CMakeFiles/make_tables.dir/depend:
	cd /home/quake/Projects/fluidsynth/fluidsynth/build/src/gentables && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/quake/Projects/fluidsynth/fluidsynth/src/gentables /home/quake/Projects/fluidsynth/fluidsynth/src/gentables /home/quake/Projects/fluidsynth/fluidsynth/build/src/gentables /home/quake/Projects/fluidsynth/fluidsynth/build/src/gentables /home/quake/Projects/fluidsynth/fluidsynth/build/src/gentables/CMakeFiles/make_tables.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/make_tables.dir/depend

