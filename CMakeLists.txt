cmake_minimum_required (VERSION 3.8.2)
project(KeyValueStorage)
add_compile_options(-std=c11)
include_directories(include)

file(GLOB sources "src/*.c")
list(REMOVE_ITEM sources "src/node.c" "src/hashtable.c" "src/thpool.c")

add_library(node "src/node.c")
add_library(hashtable "src/hashtable.c")

add_library(thpool "src/thpool.c")

add_executable(main ${sources})
add_executable(node_test "test/node_test.c")
add_executable(hash_test "test/hash_test.c")
add_executable(thpool_rwlock_test "test/thpool_rwlock_test.c")

target_link_libraries(hashtable node)
target_link_libraries(thpool pthread)
target_link_libraries(main hashtable thpool)
target_link_libraries(node_test node)
target_link_libraries(hash_test hashtable)
target_link_libraries(thpool_rwlock_test thpool)
