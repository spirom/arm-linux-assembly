cmake_minimum_required(VERSION 3.3)

project(driver C)

add_definitions(-std=c99 -mcpu=cortex-a53 -march=armv8-a )

set(SOURCE_FILES
        Main.c)

add_executable(neon_driver ${SOURCE_FILES})
target_link_libraries(neon_driver libs_64)


