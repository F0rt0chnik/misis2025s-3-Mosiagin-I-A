# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /opt/anaconda3/lib/python3.11/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /opt/anaconda3/lib/python3.11/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build

# Include any dependencies generated for this target.
include semcv/CMakeFiles/semcv.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include semcv/CMakeFiles/semcv.dir/compiler_depend.make

# Include the progress variables for this target.
include semcv/CMakeFiles/semcv.dir/progress.make

# Include the compile flags for this target's objects.
include semcv/CMakeFiles/semcv.dir/flags.make

semcv/CMakeFiles/semcv.dir/src/semcv.cpp.o: semcv/CMakeFiles/semcv.dir/flags.make
semcv/CMakeFiles/semcv.dir/src/semcv.cpp.o: /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/src/semcv.cpp
semcv/CMakeFiles/semcv.dir/src/semcv.cpp.o: semcv/CMakeFiles/semcv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object semcv/CMakeFiles/semcv.dir/src/semcv.cpp.o"
	cd /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build/semcv && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT semcv/CMakeFiles/semcv.dir/src/semcv.cpp.o -MF CMakeFiles/semcv.dir/src/semcv.cpp.o.d -o CMakeFiles/semcv.dir/src/semcv.cpp.o -c /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/src/semcv.cpp

semcv/CMakeFiles/semcv.dir/src/semcv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/semcv.dir/src/semcv.cpp.i"
	cd /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build/semcv && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/src/semcv.cpp > CMakeFiles/semcv.dir/src/semcv.cpp.i

semcv/CMakeFiles/semcv.dir/src/semcv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/semcv.dir/src/semcv.cpp.s"
	cd /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build/semcv && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/src/semcv.cpp -o CMakeFiles/semcv.dir/src/semcv.cpp.s

# Object files for target semcv
semcv_OBJECTS = \
"CMakeFiles/semcv.dir/src/semcv.cpp.o"

# External object files for target semcv
semcv_EXTERNAL_OBJECTS =

semcv/libsemcv.a: semcv/CMakeFiles/semcv.dir/src/semcv.cpp.o
semcv/libsemcv.a: semcv/CMakeFiles/semcv.dir/build.make
semcv/libsemcv.a: semcv/CMakeFiles/semcv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libsemcv.a"
	cd /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build/semcv && $(CMAKE_COMMAND) -P CMakeFiles/semcv.dir/cmake_clean_target.cmake
	cd /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build/semcv && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/semcv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
semcv/CMakeFiles/semcv.dir/build: semcv/libsemcv.a
.PHONY : semcv/CMakeFiles/semcv.dir/build

semcv/CMakeFiles/semcv.dir/clean:
	cd /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build/semcv && $(CMAKE_COMMAND) -P CMakeFiles/semcv.dir/cmake_clean.cmake
.PHONY : semcv/CMakeFiles/semcv.dir/clean

semcv/CMakeFiles/semcv.dir/depend:
	cd /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build/semcv /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/build/semcv/CMakeFiles/semcv.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : semcv/CMakeFiles/semcv.dir/depend

