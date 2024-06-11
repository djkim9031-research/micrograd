cmake_minimum_required(VERSION 3.10)
project(micrograd)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
)

set(HEADERS
    src/block.h
)

add_executable(micrograd ${SOURCES} ${HEADERS})
target_include_directories(micrograd PRIVATE "/usr/include/graphviz/")
target_link_libraries(micrograd cgraph gvc)
