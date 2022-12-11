cmake_minimum_required(VERSION 3.21.1)
project(ManualCNC)


add_executable(manual_cnc src/main.cpp)
target_include_directories(manual_cnc PUBLIC include/ PRIVATE src)


set(CMAKE_SUPPRESS_REGENERATION true)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT manual_cnc)