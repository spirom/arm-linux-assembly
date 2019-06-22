cmake_minimum_required(VERSION 3.3)

project(librbs_thumb C ASM)

add_definitions(-std=c99)

set(SOURCE_FILES
        RingBuffer.s)

include_directories(../inc)

add_library(librbs_thumb STATIC ${SOURCE_FILES})

