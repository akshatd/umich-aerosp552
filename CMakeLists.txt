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

# executables for hw1
add_executable(navigation.o hw1/navigation.cpp)
add_executable(hanoi.o hw1/hanoi.cpp)
