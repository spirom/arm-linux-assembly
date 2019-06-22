cmake_minimum_required(VERSION 3.3)

project(testrb C)

add_definitions(-std=c99)

set(SOURCE_FILES
        TestRB.c)

include_directories(../inc)

add_executable(testrbc ${SOURCE_FILES})

#add_executable(testrbs ${SOURCE_FILES})

add_executable(testrbs_64 ${SOURCE_FILES})

#add_executable(testrbs_thumb ${SOURCE_FILES})

target_link_libraries(testrbc librbc)
#target_link_libraries(testrbs librbs)
target_link_libraries(testrbs_64 librbs_64)
#target_link_libraries(testrbs_thumb librbs_thumb)

