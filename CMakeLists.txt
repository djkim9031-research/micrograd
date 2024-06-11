cmake_minimum_required(VERSION 3.10)
project(micrograd)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(SYSTEM "/usr/include/graphviz/")

# Add executable
add_executable(micrograd main.cpp)

# Link Graphviz libraries
target_link_libraries(micrograd cgraph gvc)