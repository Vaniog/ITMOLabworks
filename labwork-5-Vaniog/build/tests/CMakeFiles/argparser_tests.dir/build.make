# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/forva/CLionProjects/Labworks/labwork-5-Vaniog

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/argparser_tests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/argparser_tests.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/argparser_tests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/argparser_tests.dir/flags.make

tests/CMakeFiles/argparser_tests.dir/argparser_test.cpp.o: tests/CMakeFiles/argparser_tests.dir/flags.make
tests/CMakeFiles/argparser_tests.dir/argparser_test.cpp.o: ../tests/argparser_test.cpp
tests/CMakeFiles/argparser_tests.dir/argparser_test.cpp.o: tests/CMakeFiles/argparser_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/argparser_tests.dir/argparser_test.cpp.o"
	cd /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/argparser_tests.dir/argparser_test.cpp.o -MF CMakeFiles/argparser_tests.dir/argparser_test.cpp.o.d -o CMakeFiles/argparser_tests.dir/argparser_test.cpp.o -c /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/tests/argparser_test.cpp

tests/CMakeFiles/argparser_tests.dir/argparser_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/argparser_tests.dir/argparser_test.cpp.i"
	cd /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/tests/argparser_test.cpp > CMakeFiles/argparser_tests.dir/argparser_test.cpp.i

tests/CMakeFiles/argparser_tests.dir/argparser_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/argparser_tests.dir/argparser_test.cpp.s"
	cd /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/tests/argparser_test.cpp -o CMakeFiles/argparser_tests.dir/argparser_test.cpp.s

# Object files for target argparser_tests
argparser_tests_OBJECTS = \
"CMakeFiles/argparser_tests.dir/argparser_test.cpp.o"

# External object files for target argparser_tests
argparser_tests_EXTERNAL_OBJECTS =

tests/argparser_tests: tests/CMakeFiles/argparser_tests.dir/argparser_test.cpp.o
tests/argparser_tests: tests/CMakeFiles/argparser_tests.dir/build.make
tests/argparser_tests: lib/libargparser.a
tests/argparser_tests: lib/libgtest_main.a
tests/argparser_tests: lib/libgtest.a
tests/argparser_tests: tests/CMakeFiles/argparser_tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable argparser_tests"
	cd /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/argparser_tests.dir/link.txt --verbose=$(VERBOSE)
	cd /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/tests && /usr/bin/cmake -D TEST_TARGET=argparser_tests -D TEST_EXECUTABLE=/home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/tests/argparser_tests -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/tests -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=argparser_tests_TESTS -D CTEST_FILE=/home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/tests/argparser_tests[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /usr/share/cmake-3.22/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
tests/CMakeFiles/argparser_tests.dir/build: tests/argparser_tests
.PHONY : tests/CMakeFiles/argparser_tests.dir/build

tests/CMakeFiles/argparser_tests.dir/clean:
	cd /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/argparser_tests.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/argparser_tests.dir/clean

tests/CMakeFiles/argparser_tests.dir/depend:
	cd /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/forva/CLionProjects/Labworks/labwork-5-Vaniog /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/tests /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/tests /home/forva/CLionProjects/Labworks/labwork-5-Vaniog/build/tests/CMakeFiles/argparser_tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/argparser_tests.dir/depend

