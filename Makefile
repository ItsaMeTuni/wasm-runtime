# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /snap/cmake/1070/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1070/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lucas/dev/wasm-runtime

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lucas/dev/wasm-runtime

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/snap/cmake/1070/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/snap/cmake/1070/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/lucas/dev/wasm-runtime/CMakeFiles /home/lucas/dev/wasm-runtime//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/lucas/dev/wasm-runtime/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named wasm_runtime

# Build rule for target.
wasm_runtime: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 wasm_runtime
.PHONY : wasm_runtime

# fast build rule for target.
wasm_runtime/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/build
.PHONY : wasm_runtime/fast

Module.o: Module.cpp.o
.PHONY : Module.o

# target to build an object file
Module.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/Module.cpp.o
.PHONY : Module.cpp.o

Module.i: Module.cpp.i
.PHONY : Module.i

# target to preprocess a source file
Module.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/Module.cpp.i
.PHONY : Module.cpp.i

Module.s: Module.cpp.s
.PHONY : Module.s

# target to generate assembly for a file
Module.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/Module.cpp.s
.PHONY : Module.cpp.s

ModuleParser.o: ModuleParser.cpp.o
.PHONY : ModuleParser.o

# target to build an object file
ModuleParser.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/ModuleParser.cpp.o
.PHONY : ModuleParser.cpp.o

ModuleParser.i: ModuleParser.cpp.i
.PHONY : ModuleParser.i

# target to preprocess a source file
ModuleParser.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/ModuleParser.cpp.i
.PHONY : ModuleParser.cpp.i

ModuleParser.s: ModuleParser.cpp.s
.PHONY : ModuleParser.s

# target to generate assembly for a file
ModuleParser.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/ModuleParser.cpp.s
.PHONY : ModuleParser.cpp.s

bytecode.o: bytecode.cpp.o
.PHONY : bytecode.o

# target to build an object file
bytecode.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/bytecode.cpp.o
.PHONY : bytecode.cpp.o

bytecode.i: bytecode.cpp.i
.PHONY : bytecode.i

# target to preprocess a source file
bytecode.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/bytecode.cpp.i
.PHONY : bytecode.cpp.i

bytecode.s: bytecode.cpp.s
.PHONY : bytecode.s

# target to generate assembly for a file
bytecode.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/bytecode.cpp.s
.PHONY : bytecode.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/main.cpp.s
.PHONY : main.cpp.s

runtime.o: runtime.cpp.o
.PHONY : runtime.o

# target to build an object file
runtime.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/runtime.cpp.o
.PHONY : runtime.cpp.o

runtime.i: runtime.cpp.i
.PHONY : runtime.i

# target to preprocess a source file
runtime.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/runtime.cpp.i
.PHONY : runtime.cpp.i

runtime.s: runtime.cpp.s
.PHONY : runtime.s

# target to generate assembly for a file
runtime.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/wasm_runtime.dir/build.make CMakeFiles/wasm_runtime.dir/runtime.cpp.s
.PHONY : runtime.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... wasm_runtime"
	@echo "... Module.o"
	@echo "... Module.i"
	@echo "... Module.s"
	@echo "... ModuleParser.o"
	@echo "... ModuleParser.i"
	@echo "... ModuleParser.s"
	@echo "... bytecode.o"
	@echo "... bytecode.i"
	@echo "... bytecode.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... runtime.o"
	@echo "... runtime.i"
	@echo "... runtime.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

