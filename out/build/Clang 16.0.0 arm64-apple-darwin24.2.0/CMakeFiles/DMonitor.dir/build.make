# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /opt/homebrew/opt/cmake/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/opt/cmake/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/bagmingyu/DMonitor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0"

# Include any dependencies generated for this target.
include CMakeFiles/DMonitor.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/DMonitor.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/DMonitor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DMonitor.dir/flags.make

CMakeFiles/DMonitor.dir/codegen:
.PHONY : CMakeFiles/DMonitor.dir/codegen

CMakeFiles/DMonitor.dir/DMonitor.c.o: CMakeFiles/DMonitor.dir/flags.make
CMakeFiles/DMonitor.dir/DMonitor.c.o: /Users/bagmingyu/DMonitor/DMonitor.c
CMakeFiles/DMonitor.dir/DMonitor.c.o: CMakeFiles/DMonitor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/DMonitor.dir/DMonitor.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/DMonitor.dir/DMonitor.c.o -MF CMakeFiles/DMonitor.dir/DMonitor.c.o.d -o CMakeFiles/DMonitor.dir/DMonitor.c.o -c /Users/bagmingyu/DMonitor/DMonitor.c

CMakeFiles/DMonitor.dir/DMonitor.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/DMonitor.dir/DMonitor.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/bagmingyu/DMonitor/DMonitor.c > CMakeFiles/DMonitor.dir/DMonitor.c.i

CMakeFiles/DMonitor.dir/DMonitor.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/DMonitor.dir/DMonitor.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/bagmingyu/DMonitor/DMonitor.c -o CMakeFiles/DMonitor.dir/DMonitor.c.s

CMakeFiles/DMonitor.dir/GUIManager.c.o: CMakeFiles/DMonitor.dir/flags.make
CMakeFiles/DMonitor.dir/GUIManager.c.o: /Users/bagmingyu/DMonitor/GUIManager.c
CMakeFiles/DMonitor.dir/GUIManager.c.o: CMakeFiles/DMonitor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/DMonitor.dir/GUIManager.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/DMonitor.dir/GUIManager.c.o -MF CMakeFiles/DMonitor.dir/GUIManager.c.o.d -o CMakeFiles/DMonitor.dir/GUIManager.c.o -c /Users/bagmingyu/DMonitor/GUIManager.c

CMakeFiles/DMonitor.dir/GUIManager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/DMonitor.dir/GUIManager.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/bagmingyu/DMonitor/GUIManager.c > CMakeFiles/DMonitor.dir/GUIManager.c.i

CMakeFiles/DMonitor.dir/GUIManager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/DMonitor.dir/GUIManager.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/bagmingyu/DMonitor/GUIManager.c -o CMakeFiles/DMonitor.dir/GUIManager.c.s

CMakeFiles/DMonitor.dir/ClientManager.c.o: CMakeFiles/DMonitor.dir/flags.make
CMakeFiles/DMonitor.dir/ClientManager.c.o: /Users/bagmingyu/DMonitor/ClientManager.c
CMakeFiles/DMonitor.dir/ClientManager.c.o: CMakeFiles/DMonitor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/DMonitor.dir/ClientManager.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/DMonitor.dir/ClientManager.c.o -MF CMakeFiles/DMonitor.dir/ClientManager.c.o.d -o CMakeFiles/DMonitor.dir/ClientManager.c.o -c /Users/bagmingyu/DMonitor/ClientManager.c

CMakeFiles/DMonitor.dir/ClientManager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/DMonitor.dir/ClientManager.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/bagmingyu/DMonitor/ClientManager.c > CMakeFiles/DMonitor.dir/ClientManager.c.i

CMakeFiles/DMonitor.dir/ClientManager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/DMonitor.dir/ClientManager.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/bagmingyu/DMonitor/ClientManager.c -o CMakeFiles/DMonitor.dir/ClientManager.c.s

CMakeFiles/DMonitor.dir/TabPanel.c.o: CMakeFiles/DMonitor.dir/flags.make
CMakeFiles/DMonitor.dir/TabPanel.c.o: /Users/bagmingyu/DMonitor/TabPanel.c
CMakeFiles/DMonitor.dir/TabPanel.c.o: CMakeFiles/DMonitor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/DMonitor.dir/TabPanel.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/DMonitor.dir/TabPanel.c.o -MF CMakeFiles/DMonitor.dir/TabPanel.c.o.d -o CMakeFiles/DMonitor.dir/TabPanel.c.o -c /Users/bagmingyu/DMonitor/TabPanel.c

CMakeFiles/DMonitor.dir/TabPanel.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/DMonitor.dir/TabPanel.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/bagmingyu/DMonitor/TabPanel.c > CMakeFiles/DMonitor.dir/TabPanel.c.i

CMakeFiles/DMonitor.dir/TabPanel.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/DMonitor.dir/TabPanel.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/bagmingyu/DMonitor/TabPanel.c -o CMakeFiles/DMonitor.dir/TabPanel.c.s

CMakeFiles/DMonitor.dir/ServerLauncher.c.o: CMakeFiles/DMonitor.dir/flags.make
CMakeFiles/DMonitor.dir/ServerLauncher.c.o: /Users/bagmingyu/DMonitor/ServerLauncher.c
CMakeFiles/DMonitor.dir/ServerLauncher.c.o: CMakeFiles/DMonitor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/DMonitor.dir/ServerLauncher.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/DMonitor.dir/ServerLauncher.c.o -MF CMakeFiles/DMonitor.dir/ServerLauncher.c.o.d -o CMakeFiles/DMonitor.dir/ServerLauncher.c.o -c /Users/bagmingyu/DMonitor/ServerLauncher.c

CMakeFiles/DMonitor.dir/ServerLauncher.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/DMonitor.dir/ServerLauncher.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/bagmingyu/DMonitor/ServerLauncher.c > CMakeFiles/DMonitor.dir/ServerLauncher.c.i

CMakeFiles/DMonitor.dir/ServerLauncher.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/DMonitor.dir/ServerLauncher.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/bagmingyu/DMonitor/ServerLauncher.c -o CMakeFiles/DMonitor.dir/ServerLauncher.c.s

CMakeFiles/DMonitor.dir/DMonitorThread.c.o: CMakeFiles/DMonitor.dir/flags.make
CMakeFiles/DMonitor.dir/DMonitorThread.c.o: /Users/bagmingyu/DMonitor/DMonitorThread.c
CMakeFiles/DMonitor.dir/DMonitorThread.c.o: CMakeFiles/DMonitor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/DMonitor.dir/DMonitorThread.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/DMonitor.dir/DMonitorThread.c.o -MF CMakeFiles/DMonitor.dir/DMonitorThread.c.o.d -o CMakeFiles/DMonitor.dir/DMonitorThread.c.o -c /Users/bagmingyu/DMonitor/DMonitorThread.c

CMakeFiles/DMonitor.dir/DMonitorThread.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/DMonitor.dir/DMonitorThread.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/bagmingyu/DMonitor/DMonitorThread.c > CMakeFiles/DMonitor.dir/DMonitorThread.c.i

CMakeFiles/DMonitor.dir/DMonitorThread.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/DMonitor.dir/DMonitorThread.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/bagmingyu/DMonitor/DMonitorThread.c -o CMakeFiles/DMonitor.dir/DMonitorThread.c.s

# Object files for target DMonitor
DMonitor_OBJECTS = \
"CMakeFiles/DMonitor.dir/DMonitor.c.o" \
"CMakeFiles/DMonitor.dir/GUIManager.c.o" \
"CMakeFiles/DMonitor.dir/ClientManager.c.o" \
"CMakeFiles/DMonitor.dir/TabPanel.c.o" \
"CMakeFiles/DMonitor.dir/ServerLauncher.c.o" \
"CMakeFiles/DMonitor.dir/DMonitorThread.c.o"

# External object files for target DMonitor
DMonitor_EXTERNAL_OBJECTS =

DMonitor: CMakeFiles/DMonitor.dir/DMonitor.c.o
DMonitor: CMakeFiles/DMonitor.dir/GUIManager.c.o
DMonitor: CMakeFiles/DMonitor.dir/ClientManager.c.o
DMonitor: CMakeFiles/DMonitor.dir/TabPanel.c.o
DMonitor: CMakeFiles/DMonitor.dir/ServerLauncher.c.o
DMonitor: CMakeFiles/DMonitor.dir/DMonitorThread.c.o
DMonitor: CMakeFiles/DMonitor.dir/build.make
DMonitor: /Library/Developer/CommandLineTools/SDKs/MacOSX15.1.sdk/usr/lib/libcurses.tbd
DMonitor: /Library/Developer/CommandLineTools/SDKs/MacOSX15.1.sdk/usr/lib/libform.tbd
DMonitor: CMakeFiles/DMonitor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Linking C executable DMonitor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DMonitor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DMonitor.dir/build: DMonitor
.PHONY : CMakeFiles/DMonitor.dir/build

CMakeFiles/DMonitor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DMonitor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DMonitor.dir/clean

CMakeFiles/DMonitor.dir/depend:
	cd "/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/bagmingyu/DMonitor /Users/bagmingyu/DMonitor "/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0" "/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0" "/Users/bagmingyu/DMonitor/out/build/Clang 16.0.0 arm64-apple-darwin24.2.0/CMakeFiles/DMonitor.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/DMonitor.dir/depend

