cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

# Locate GTest
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

# Add the testing executable
# add_executable(test test/utest_system_monitor.cpp src/linux_parser.cpp src/format.cpp src/system.cpp src/processor.cpp)
# target_link_libraries(test ${GTEST_LIBRARIES} pthread)

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
