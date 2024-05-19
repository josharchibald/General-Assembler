cmake_minimum_required(VERSION 3.10)

# Set the project name
project(GenA)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-long-long -Wno-variadic-macros -fexceptions -g")

# Handle debug and release builds
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find all source files in src and lib directories
file(GLOB SOURCES "src/*.cpp" "lib/*.cpp")

# Add the executable
add_executable(gena ${SOURCES})

# Post-build command to move the executable to /usr/local/bin/ using sudo
add_custom_command(TARGET gena POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E env "SHELL=/bin/bash" sudo ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gena> /usr/local/bin/gena
)

# Custom clean target
add_custom_target(clean_all
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/*.o
  COMMAND sudo rm -f /usr/local/bin/gena
)
