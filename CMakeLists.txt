cmake_minimum_required (VERSION 3.8.2)
project(KeyValueStorage)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(main ${SOURCES})
add_executable(node_test "test/node_test.c" "src/node.c")
add_executable(hash_test "test/hash_test.c" "src/node.c" "src/hashtable.c")
