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
CMAKE_SOURCE_DIR = /home/zhushanbo/download/Zeegine-Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zhushanbo/download/Zeegine-Server/build

# Include any dependencies generated for this target.
include CMakeFiles/ZeegineServer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ZeegineServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ZeegineServer.dir/flags.make

CMakeFiles/ZeegineServer.dir/src/main.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/main.cpp.o: ../src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/main.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/main.cpp

CMakeFiles/ZeegineServer.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/main.cpp > CMakeFiles/ZeegineServer.dir/src/main.cpp.i

CMakeFiles/ZeegineServer.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/main.cpp -o CMakeFiles/ZeegineServer.dir/src/main.cpp.s

CMakeFiles/ZeegineServer.dir/src/main.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/main.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/main.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/main.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/main.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/main.cpp.o

CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o: ../src/AviGenerator.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/AviGenerator.cpp

CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/AviGenerator.cpp > CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.i

CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/AviGenerator.cpp -o CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.s

CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o

CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o: ../src/GCode.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/GCode.cpp

CMakeFiles/ZeegineServer.dir/src/GCode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/GCode.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/GCode.cpp > CMakeFiles/ZeegineServer.dir/src/GCode.cpp.i

CMakeFiles/ZeegineServer.dir/src/GCode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/GCode.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/GCode.cpp -o CMakeFiles/ZeegineServer.dir/src/GCode.cpp.s

CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o

CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o: ../src/PrinterState.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/PrinterState.cpp

CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/PrinterState.cpp > CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.i

CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/PrinterState.cpp -o CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.s

CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o

CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o: ../src/WebserverAPI.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/WebserverAPI.cpp

CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/WebserverAPI.cpp > CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.i

CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/WebserverAPI.cpp -o CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.s

CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o

CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o: ../src/jpg2mp4.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/jpg2mp4.cpp

CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/jpg2mp4.cpp > CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.i

CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/jpg2mp4.cpp -o CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.s

CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o

CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o: ../src/global_config.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/global_config.cpp

CMakeFiles/ZeegineServer.dir/src/global_config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/global_config.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/global_config.cpp > CMakeFiles/ZeegineServer.dir/src/global_config.cpp.i

CMakeFiles/ZeegineServer.dir/src/global_config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/global_config.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/global_config.cpp -o CMakeFiles/ZeegineServer.dir/src/global_config.cpp.s

CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o

CMakeFiles/ZeegineServer.dir/src/printer.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/printer.cpp.o: ../src/printer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/printer.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/printer.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/printer.cpp

CMakeFiles/ZeegineServer.dir/src/printer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/printer.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/printer.cpp > CMakeFiles/ZeegineServer.dir/src/printer.cpp.i

CMakeFiles/ZeegineServer.dir/src/printer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/printer.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/printer.cpp -o CMakeFiles/ZeegineServer.dir/src/printer.cpp.s

CMakeFiles/ZeegineServer.dir/src/printer.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/printer.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/printer.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/printer.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/printer.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/printer.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/printer.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/printer.cpp.o

CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o: ../src/Printjob.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/Printjob.cpp

CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/Printjob.cpp > CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.i

CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/Printjob.cpp -o CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.s

CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o

CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o: ../src/RLog.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/RLog.cpp

CMakeFiles/ZeegineServer.dir/src/RLog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/RLog.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/RLog.cpp > CMakeFiles/ZeegineServer.dir/src/RLog.cpp.i

CMakeFiles/ZeegineServer.dir/src/RLog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/RLog.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/RLog.cpp -o CMakeFiles/ZeegineServer.dir/src/RLog.cpp.s

CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o

CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o: ../src/PrinterSerial.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/PrinterSerial.cpp

CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/PrinterSerial.cpp > CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.i

CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/PrinterSerial.cpp -o CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.s

CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o

CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o: CMakeFiles/ZeegineServer.dir/flags.make
CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o: ../src/timeEstimate.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o -c /home/zhushanbo/download/Zeegine-Server/src/timeEstimate.cpp

CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zhushanbo/download/Zeegine-Server/src/timeEstimate.cpp > CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.i

CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zhushanbo/download/Zeegine-Server/src/timeEstimate.cpp -o CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.s

CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o.requires:
.PHONY : CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o.requires

CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o.provides: CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o.requires
	$(MAKE) -f CMakeFiles/ZeegineServer.dir/build.make CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o.provides.build
.PHONY : CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o.provides

CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o.provides.build: CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o

# Object files for target ZeegineServer
ZeegineServer_OBJECTS = \
"CMakeFiles/ZeegineServer.dir/src/main.cpp.o" \
"CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o" \
"CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o" \
"CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o" \
"CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o" \
"CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o" \
"CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o" \
"CMakeFiles/ZeegineServer.dir/src/printer.cpp.o" \
"CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o" \
"CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o" \
"CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o" \
"CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o"

# External object files for target ZeegineServer
ZeegineServer_EXTERNAL_OBJECTS =

ZeegineServer: CMakeFiles/ZeegineServer.dir/src/main.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/src/printer.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o
ZeegineServer: CMakeFiles/ZeegineServer.dir/build.make
ZeegineServer: /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
ZeegineServer: /usr/lib/x86_64-linux-gnu/libboost_thread.a
ZeegineServer: /usr/lib/x86_64-linux-gnu/libboost_program_options.a
ZeegineServer: /usr/lib/x86_64-linux-gnu/libboost_system.a
ZeegineServer: /usr/lib/x86_64-linux-gnu/libboost_date_time.a
ZeegineServer: /usr/lib/x86_64-linux-gnu/libboost_chrono.a
ZeegineServer: /usr/lib/x86_64-linux-gnu/libboost_regex.a
ZeegineServer: /usr/lib/x86_64-linux-gnu/libpthread.so
ZeegineServer: lib/json_spirit/libjson_spirit_x86.a
ZeegineServer: lib/mongoose/libmongoose_x86.a
ZeegineServer: lib/moFileReader/libmoFileReader_x86.a
ZeegineServer: lib/libconfig/libconfig_x86.a
ZeegineServer: CMakeFiles/ZeegineServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ZeegineServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ZeegineServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ZeegineServer.dir/build: ZeegineServer
.PHONY : CMakeFiles/ZeegineServer.dir/build

CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/main.cpp.o.requires
CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/AviGenerator.cpp.o.requires
CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/GCode.cpp.o.requires
CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/PrinterState.cpp.o.requires
CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/WebserverAPI.cpp.o.requires
CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/jpg2mp4.cpp.o.requires
CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/global_config.cpp.o.requires
CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/printer.cpp.o.requires
CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/Printjob.cpp.o.requires
CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/RLog.cpp.o.requires
CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/PrinterSerial.cpp.o.requires
CMakeFiles/ZeegineServer.dir/requires: CMakeFiles/ZeegineServer.dir/src/timeEstimate.cpp.o.requires
.PHONY : CMakeFiles/ZeegineServer.dir/requires

CMakeFiles/ZeegineServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ZeegineServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ZeegineServer.dir/clean

CMakeFiles/ZeegineServer.dir/depend:
	cd /home/zhushanbo/download/Zeegine-Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhushanbo/download/Zeegine-Server /home/zhushanbo/download/Zeegine-Server /home/zhushanbo/download/Zeegine-Server/build /home/zhushanbo/download/Zeegine-Server/build /home/zhushanbo/download/Zeegine-Server/build/CMakeFiles/ZeegineServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ZeegineServer.dir/depend

