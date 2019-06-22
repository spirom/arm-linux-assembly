cmake_minimum_required(VERSION 3.3)
project(arm_linux_assembly)

ADD_SUBDIRECTORY(ring-buffer)
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
ADD_SUBDIRECTORY(neon)
endif ()
