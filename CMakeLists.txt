cmake_minimum_required(VERSION 3.10.2)

project(Deku VERSION 1.0)

add_executable(Deku src/start.cxx)

configure_file(version_control.h.in version_control.h)

target_include_directories(Deku PUBLIC "${PROJECT_BINARY_DIR}")
