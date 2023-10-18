cmake_minimum_required(VERSION 3.24)

project(ae552)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif ()
# https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md#gcc--clang
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -pedantic -Wshadow -Wnon-virtual-dtor \
										-Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion \
										-Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference \
										-Wuseless-cast -Wdouble-promotion -Wformat=2 -Wpadded")

# setup some vars
set(INCLUDE_DIR include)
set(TEST_DIR test)

# execs for hw1
add_executable(navigation hw1/navigation.cpp)
add_executable(hanoi hw1/hanoi.cpp)

add_library(libtree hw1/Tree.cpp)
add_executable(tree hw1/tree_main.cpp)
target_link_libraries(tree PUBLIC libtree)

add_executable(life hw1/life.cpp)

# execs for hw2
add_executable(automaton hw2/automaton.cpp)

# execs for hw3
add_executable(turing hw3/turing.cpp)
