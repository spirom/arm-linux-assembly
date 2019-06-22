cmake_minimum_required(VERSION 3.3)

project(librbc C)

add_definitions(-std=c99)

set(SOURCE_FILES
	RingBuffer.c)

include_directories(../inc)

add_library(librbc STATIC ${SOURCE_FILES})

target_include_directories(librbc PUBLIC ../inc)
