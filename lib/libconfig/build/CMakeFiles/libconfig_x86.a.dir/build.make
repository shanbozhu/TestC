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
CMAKE_SOURCE_DIR = /home/zhushanbo/download/Zeegine-Server/lib/libconfig

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zhushanbo/download/Zeegine-Server/lib/libconfig/build

# Include any dependencies generated for this target.
include CMakeFiles/libconfig_x86.a.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/libconfig_x86.a.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/libconfig_x86.a.dir/flags.make

CMakeFiles/libconfig_x86.a.dir/scanner.c.o: CMakeFiles/libconfig_x86.a.dir/flags.make
CMakeFiles/libconfig_x86.a.dir/scanner.c.o: ../scanner.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/lib/libconfig/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/libconfig_x86.a.dir/scanner.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/libconfig_x86.a.dir/scanner.c.o   -c /home/zhushanbo/download/Zeegine-Server/lib/libconfig/scanner.c

CMakeFiles/libconfig_x86.a.dir/scanner.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libconfig_x86.a.dir/scanner.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/lib/libconfig/scanner.c > CMakeFiles/libconfig_x86.a.dir/scanner.c.i

CMakeFiles/libconfig_x86.a.dir/scanner.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libconfig_x86.a.dir/scanner.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/lib/libconfig/scanner.c -o CMakeFiles/libconfig_x86.a.dir/scanner.c.s

CMakeFiles/libconfig_x86.a.dir/scanner.c.o.requires:
.PHONY : CMakeFiles/libconfig_x86.a.dir/scanner.c.o.requires

CMakeFiles/libconfig_x86.a.dir/scanner.c.o.provides: CMakeFiles/libconfig_x86.a.dir/scanner.c.o.requires
	$(MAKE) -f CMakeFiles/libconfig_x86.a.dir/build.make CMakeFiles/libconfig_x86.a.dir/scanner.c.o.provides.build
.PHONY : CMakeFiles/libconfig_x86.a.dir/scanner.c.o.provides

CMakeFiles/libconfig_x86.a.dir/scanner.c.o.provides.build: CMakeFiles/libconfig_x86.a.dir/scanner.c.o

CMakeFiles/libconfig_x86.a.dir/libconfig.c.o: CMakeFiles/libconfig_x86.a.dir/flags.make
CMakeFiles/libconfig_x86.a.dir/libconfig.c.o: ../libconfig.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/lib/libconfig/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/libconfig_x86.a.dir/libconfig.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/libconfig_x86.a.dir/libconfig.c.o   -c /home/zhushanbo/download/Zeegine-Server/lib/libconfig/libconfig.c

CMakeFiles/libconfig_x86.a.dir/libconfig.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libconfig_x86.a.dir/libconfig.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/lib/libconfig/libconfig.c > CMakeFiles/libconfig_x86.a.dir/libconfig.c.i

CMakeFiles/libconfig_x86.a.dir/libconfig.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libconfig_x86.a.dir/libconfig.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/lib/libconfig/libconfig.c -o CMakeFiles/libconfig_x86.a.dir/libconfig.c.s

CMakeFiles/libconfig_x86.a.dir/libconfig.c.o.requires:
.PHONY : CMakeFiles/libconfig_x86.a.dir/libconfig.c.o.requires

CMakeFiles/libconfig_x86.a.dir/libconfig.c.o.provides: CMakeFiles/libconfig_x86.a.dir/libconfig.c.o.requires
	$(MAKE) -f CMakeFiles/libconfig_x86.a.dir/build.make CMakeFiles/libconfig_x86.a.dir/libconfig.c.o.provides.build
.PHONY : CMakeFiles/libconfig_x86.a.dir/libconfig.c.o.provides

CMakeFiles/libconfig_x86.a.dir/libconfig.c.o.provides.build: CMakeFiles/libconfig_x86.a.dir/libconfig.c.o

CMakeFiles/libconfig_x86.a.dir/grammar.c.o: CMakeFiles/libconfig_x86.a.dir/flags.make
CMakeFiles/libconfig_x86.a.dir/grammar.c.o: ../grammar.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/lib/libconfig/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/libconfig_x86.a.dir/grammar.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/libconfig_x86.a.dir/grammar.c.o   -c /home/zhushanbo/download/Zeegine-Server/lib/libconfig/grammar.c

CMakeFiles/libconfig_x86.a.dir/grammar.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libconfig_x86.a.dir/grammar.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/lib/libconfig/grammar.c > CMakeFiles/libconfig_x86.a.dir/grammar.c.i

CMakeFiles/libconfig_x86.a.dir/grammar.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libconfig_x86.a.dir/grammar.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/lib/libconfig/grammar.c -o CMakeFiles/libconfig_x86.a.dir/grammar.c.s

CMakeFiles/libconfig_x86.a.dir/grammar.c.o.requires:
.PHONY : CMakeFiles/libconfig_x86.a.dir/grammar.c.o.requires

CMakeFiles/libconfig_x86.a.dir/grammar.c.o.provides: CMakeFiles/libconfig_x86.a.dir/grammar.c.o.requires
	$(MAKE) -f CMakeFiles/libconfig_x86.a.dir/build.make CMakeFiles/libconfig_x86.a.dir/grammar.c.o.provides.build
.PHONY : CMakeFiles/libconfig_x86.a.dir/grammar.c.o.provides

CMakeFiles/libconfig_x86.a.dir/grammar.c.o.provides.build: CMakeFiles/libconfig_x86.a.dir/grammar.c.o

CMakeFiles/libconfig_x86.a.dir/strbuf.c.o: CMakeFiles/libconfig_x86.a.dir/flags.make
CMakeFiles/libconfig_x86.a.dir/strbuf.c.o: ../strbuf.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/lib/libconfig/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/libconfig_x86.a.dir/strbuf.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/libconfig_x86.a.dir/strbuf.c.o   -c /home/zhushanbo/download/Zeegine-Server/lib/libconfig/strbuf.c

CMakeFiles/libconfig_x86.a.dir/strbuf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libconfig_x86.a.dir/strbuf.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/lib/libconfig/strbuf.c > CMakeFiles/libconfig_x86.a.dir/strbuf.c.i

CMakeFiles/libconfig_x86.a.dir/strbuf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libconfig_x86.a.dir/strbuf.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/lib/libconfig/strbuf.c -o CMakeFiles/libconfig_x86.a.dir/strbuf.c.s

CMakeFiles/libconfig_x86.a.dir/strbuf.c.o.requires:
.PHONY : CMakeFiles/libconfig_x86.a.dir/strbuf.c.o.requires

CMakeFiles/libconfig_x86.a.dir/strbuf.c.o.provides: CMakeFiles/libconfig_x86.a.dir/strbuf.c.o.requires
	$(MAKE) -f CMakeFiles/libconfig_x86.a.dir/build.make CMakeFiles/libconfig_x86.a.dir/strbuf.c.o.provides.build
.PHONY : CMakeFiles/libconfig_x86.a.dir/strbuf.c.o.provides

CMakeFiles/libconfig_x86.a.dir/strbuf.c.o.provides.build: CMakeFiles/libconfig_x86.a.dir/strbuf.c.o

CMakeFiles/libconfig_x86.a.dir/scanctx.c.o: CMakeFiles/libconfig_x86.a.dir/flags.make
CMakeFiles/libconfig_x86.a.dir/scanctx.c.o: ../scanctx.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/lib/libconfig/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/libconfig_x86.a.dir/scanctx.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/libconfig_x86.a.dir/scanctx.c.o   -c /home/zhushanbo/download/Zeegine-Server/lib/libconfig/scanctx.c

CMakeFiles/libconfig_x86.a.dir/scanctx.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libconfig_x86.a.dir/scanctx.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/lib/libconfig/scanctx.c > CMakeFiles/libconfig_x86.a.dir/scanctx.c.i

CMakeFiles/libconfig_x86.a.dir/scanctx.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libconfig_x86.a.dir/scanctx.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/lib/libconfig/scanctx.c -o CMakeFiles/libconfig_x86.a.dir/scanctx.c.s

CMakeFiles/libconfig_x86.a.dir/scanctx.c.o.requires:
.PHONY : CMakeFiles/libconfig_x86.a.dir/scanctx.c.o.requires

CMakeFiles/libconfig_x86.a.dir/scanctx.c.o.provides: CMakeFiles/libconfig_x86.a.dir/scanctx.c.o.requires
	$(MAKE) -f CMakeFiles/libconfig_x86.a.dir/build.make CMakeFiles/libconfig_x86.a.dir/scanctx.c.o.provides.build
.PHONY : CMakeFiles/libconfig_x86.a.dir/scanctx.c.o.provides

CMakeFiles/libconfig_x86.a.dir/scanctx.c.o.provides.build: CMakeFiles/libconfig_x86.a.dir/scanctx.c.o

CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o: CMakeFiles/libconfig_x86.a.dir/flags.make
CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o: ../libconfigcpp.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/lib/libconfig/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o -c /home/zhushanbo/download/Zeegine-Server/lib/libconfig/libconfigcpp.cpp

CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/lib/libconfig/libconfigcpp.cpp > CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.i

CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/lib/libconfig/libconfigcpp.cpp -o CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.s

CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o.requires:
.PHONY : CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o.requires

CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o.provides: CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o.requires
	$(MAKE) -f CMakeFiles/libconfig_x86.a.dir/build.make CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o.provides.build
.PHONY : CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o.provides

CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o.provides.build: CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o

# Object files for target libconfig_x86.a
libconfig_x86_a_OBJECTS = \
"CMakeFiles/libconfig_x86.a.dir/scanner.c.o" \
"CMakeFiles/libconfig_x86.a.dir/libconfig.c.o" \
"CMakeFiles/libconfig_x86.a.dir/grammar.c.o" \
"CMakeFiles/libconfig_x86.a.dir/strbuf.c.o" \
"CMakeFiles/libconfig_x86.a.dir/scanctx.c.o" \
"CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o"

# External object files for target libconfig_x86.a
libconfig_x86_a_EXTERNAL_OBJECTS =

libconfig_x86.a: CMakeFiles/libconfig_x86.a.dir/scanner.c.o
libconfig_x86.a: CMakeFiles/libconfig_x86.a.dir/libconfig.c.o
libconfig_x86.a: CMakeFiles/libconfig_x86.a.dir/grammar.c.o
libconfig_x86.a: CMakeFiles/libconfig_x86.a.dir/strbuf.c.o
libconfig_x86.a: CMakeFiles/libconfig_x86.a.dir/scanctx.c.o
libconfig_x86.a: CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o
libconfig_x86.a: CMakeFiles/libconfig_x86.a.dir/build.make
libconfig_x86.a: CMakeFiles/libconfig_x86.a.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libconfig_x86.a"
	$(CMAKE_COMMAND) -P CMakeFiles/libconfig_x86.a.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libconfig_x86.a.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/libconfig_x86.a.dir/build: libconfig_x86.a
.PHONY : CMakeFiles/libconfig_x86.a.dir/build

CMakeFiles/libconfig_x86.a.dir/requires: CMakeFiles/libconfig_x86.a.dir/scanner.c.o.requires
CMakeFiles/libconfig_x86.a.dir/requires: CMakeFiles/libconfig_x86.a.dir/libconfig.c.o.requires
CMakeFiles/libconfig_x86.a.dir/requires: CMakeFiles/libconfig_x86.a.dir/grammar.c.o.requires
CMakeFiles/libconfig_x86.a.dir/requires: CMakeFiles/libconfig_x86.a.dir/strbuf.c.o.requires
CMakeFiles/libconfig_x86.a.dir/requires: CMakeFiles/libconfig_x86.a.dir/scanctx.c.o.requires
CMakeFiles/libconfig_x86.a.dir/requires: CMakeFiles/libconfig_x86.a.dir/libconfigcpp.cpp.o.requires
.PHONY : CMakeFiles/libconfig_x86.a.dir/requires

CMakeFiles/libconfig_x86.a.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/libconfig_x86.a.dir/cmake_clean.cmake
.PHONY : CMakeFiles/libconfig_x86.a.dir/clean

CMakeFiles/libconfig_x86.a.dir/depend:
	cd /home/zhushanbo/download/Zeegine-Server/lib/libconfig/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhushanbo/download/Zeegine-Server/lib/libconfig /home/zhushanbo/download/Zeegine-Server/lib/libconfig /home/zhushanbo/download/Zeegine-Server/lib/libconfig/build /home/zhushanbo/download/Zeegine-Server/lib/libconfig/build /home/zhushanbo/download/Zeegine-Server/lib/libconfig/build/CMakeFiles/libconfig_x86.a.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/libconfig_x86.a.dir/depend
