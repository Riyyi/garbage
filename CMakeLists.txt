# ------------------------------------------
# User config between these lines

# Set project name
set(PROJECT "garbage")

# Unit tests
option(GARBAGE_BUILD_TESTS "Build the GarbAGE test programs" ON)

# ------------------------------------------

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(${PROJECT} CXX)

# ------------------------------------------
# Setup C++ compiler

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags used for all build types
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
# -Wall      = All warnings about contructions that are easily avoidable
# -Wextra    = Extra warning flags not covered by -Wall
# -Wpedantic = Warnings for compiler extensions not part of the standard

# Set default build type if not specified
set(DEFAULT_BUILD_TYPE Release)
if(EXISTS ${CMAKE_SOURCE_DIR}/.git)
	set(DEFAULT_BUILD_TYPE Debug)
endif()
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE})
endif()

# Set build type specific compiler flags
message("--- ${CMAKE_BUILD_TYPE} ---")
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g -pg")
	# -Og = Optimizations that do not interfere with debugging
	# -g  = Produce debugging information in OS's native format
	# -pg = Generate profile information for analysis with gprof
	# $ gprof <PROJECT> gmon.out > profile-data.txt
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	# -O3 = Optimizations that increase compilation time and performance
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------
# Library

set(INFERNO_BUILD_EXAMPLES OFF)
add_subdirectory("vendor/inferno")

# ------------------------------------------
# Application target

# Define source files
file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp")

add_executable(${PROJECT} ${PROJECT_SOURCES})
target_include_directories(${PROJECT} PRIVATE
	"src")
target_link_libraries(${PROJECT} inferno)

# ------------------------------------------
# Assets target

add_custom_target(${PROJECT}-assets
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy-assets.cmake
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_dependencies(${PROJECT} ${PROJECT}-assets)
add_dependencies(${PROJECT}-assets inferno-assets) # Copy project assets *AFTER* engine assets!

# ------------------------------------------
# Execute target

add_custom_target(run
    COMMAND ${PROJECT})
add_dependencies(run ${PROJECT})

# ------------------------------------------
# Unit test target

if (GARBAGE_BUILD_TESTS)
	# Define test source files
	file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
	set(TEST_SOURCES ${TEST_SOURCES} ${PROJECT_SOURCES})
	list(REMOVE_ITEM TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

	add_executable(${PROJECT}-unit-test ${TEST_SOURCES})
	target_include_directories(${PROJECT}-unit-test PRIVATE
		"src"
		"test")
	target_link_libraries(${PROJECT}-unit-test inferno)
endif()
