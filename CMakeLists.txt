cmake_minimum_required (VERSION 3.8.2)
project(KeyValueStorage)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(main ${SOURCES})