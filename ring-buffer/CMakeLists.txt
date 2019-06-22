cmake_minimum_required(VERSION 3.3)
project(ringbuffer)

add_compile_options("-g")

ADD_SUBDIRECTORY(clib)
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
ADD_SUBDIRECTORY(slib_64)
else ()
ADD_SUBDIRECTORY(slib)
ADD_SUBDIRECTORY(slib_thumb)
endif ()
ADD_SUBDIRECTORY(test)


