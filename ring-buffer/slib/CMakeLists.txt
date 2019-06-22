cmake_minimum_required(VERSION 3.3)

project(librbs C ASM)

set(SOURCE_FILES
        RingBuffer.s)

SET(CMAKE_ASM_FLAGS "${CFLAGS} -march=armv8-a")

add_library(librbs STATIC ${SOURCE_FILES})

