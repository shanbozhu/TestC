# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/zhushanbo/download/Zeegine-Server/lib/json_spirit

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/build

# Include any dependencies generated for this target.
include CMakeFiles/libjson_spirit_x86.a.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/libjson_spirit_x86.a.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/libjson_spirit_x86.a.dir/flags.make

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o: CMakeFiles/libjson_spirit_x86.a.dir/flags.make
CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o: ../json_spirit_reader.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o -c /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/json_spirit_reader.cpp

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/json_spirit_reader.cpp > CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.i

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/json_spirit_reader.cpp -o CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.s

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o.requires:
.PHONY : CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o.requires

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o.provides: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o.requires
	$(MAKE) -f CMakeFiles/libjson_spirit_x86.a.dir/build.make CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o.provides.build
.PHONY : CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o.provides

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o.provides.build: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o: CMakeFiles/libjson_spirit_x86.a.dir/flags.make
CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o: ../json_spirit_writer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o -c /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/json_spirit_writer.cpp

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/json_spirit_writer.cpp > CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.i

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/json_spirit_writer.cpp -o CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.s

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o.requires:
.PHONY : CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o.requires

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o.provides: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o.requires
	$(MAKE) -f CMakeFiles/libjson_spirit_x86.a.dir/build.make CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o.provides.build
.PHONY : CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o.provides

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o.provides.build: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o: CMakeFiles/libjson_spirit_x86.a.dir/flags.make
CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o: ../json_spirit_value.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o -c /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/json_spirit_value.cpp

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/json_spirit_value.cpp > CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.i

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/json_spirit_value.cpp -o CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.s

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o.requires:
.PHONY : CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o.requires

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o.provides: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o.requires
	$(MAKE) -f CMakeFiles/libjson_spirit_x86.a.dir/build.make CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o.provides.build
.PHONY : CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o.provides

CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o.provides.build: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o

# Object files for target libjson_spirit_x86.a
libjson_spirit_x86_a_OBJECTS = \
"CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o" \
"CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o" \
"CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o"

# External object files for target libjson_spirit_x86.a
libjson_spirit_x86_a_EXTERNAL_OBJECTS =

libjson_spirit_x86.a: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o
libjson_spirit_x86.a: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o
libjson_spirit_x86.a: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o
libjson_spirit_x86.a: CMakeFiles/libjson_spirit_x86.a.dir/build.make
libjson_spirit_x86.a: CMakeFiles/libjson_spirit_x86.a.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libjson_spirit_x86.a"
	$(CMAKE_COMMAND) -P CMakeFiles/libjson_spirit_x86.a.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libjson_spirit_x86.a.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/libjson_spirit_x86.a.dir/build: libjson_spirit_x86.a
.PHONY : CMakeFiles/libjson_spirit_x86.a.dir/build

CMakeFiles/libjson_spirit_x86.a.dir/requires: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_reader.cpp.o.requires
CMakeFiles/libjson_spirit_x86.a.dir/requires: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_writer.cpp.o.requires
CMakeFiles/libjson_spirit_x86.a.dir/requires: CMakeFiles/libjson_spirit_x86.a.dir/json_spirit_value.cpp.o.requires
.PHONY : CMakeFiles/libjson_spirit_x86.a.dir/requires

CMakeFiles/libjson_spirit_x86.a.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/libjson_spirit_x86.a.dir/cmake_clean.cmake
.PHONY : CMakeFiles/libjson_spirit_x86.a.dir/clean

CMakeFiles/libjson_spirit_x86.a.dir/depend:
	cd /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhushanbo/download/Zeegine-Server/lib/json_spirit /home/zhushanbo/download/Zeegine-Server/lib/json_spirit /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/build /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/build /home/zhushanbo/download/Zeegine-Server/lib/json_spirit/build/CMakeFiles/libjson_spirit_x86.a.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/libjson_spirit_x86.a.dir/depend

