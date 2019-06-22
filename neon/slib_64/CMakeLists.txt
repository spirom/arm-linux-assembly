cmake_minimum_required(VERSION 3.3)

project(libs64 ASM)

set(SOURCE_FILES
	VectorAdd.s)

SET(CMAKE_ASM_FLAGS "${CFLAGS} -march=armv8-a -g")

add_library(libs_64 STATIC ${SOURCE_FILES})

