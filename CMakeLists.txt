cmake_minimum_required (VERSION 3.5.1)
project(KeyValueStorage)
include_directories(include)

file(GLOB sources "src/*.c")
list(REMOVE_ITEM sources "${CMAKE_CURRENT_SOURCE_DIR}/src/node.c"
                         "${CMAKE_CURRENT_SOURCE_DIR}/src/hashtable.c"
                         "${CMAKE_CURRENT_SOURCE_DIR}/src/thpool.c")

add_library(node "src/node.c")
add_library(hashtable "src/hashtable.c")

add_library(thpool "src/thpool.c")

add_executable(main ${sources})
add_executable(node_test "test/node_test.c")
add_executable(hashtable_test "test/hash_test.c")
add_executable(thpool_rwlock_test "test/thpool_rwlock_test.c")
add_executable(client_test "test/client_test.c" "src/client.c" "src/server.c")

target_link_libraries(hashtable node)
target_link_libraries(thpool pthread)
target_link_libraries(main hashtable thpool)
target_link_libraries(node_test node)
target_link_libraries(hashtable_test hashtable)
target_link_libraries(thpool_rwlock_test thpool)
target_link_libraries(client_test thpool hashtable)

target_compile_definitions(client_test PUBLIC CLIENT_TEST)
target_compile_options(client_test PRIVATE "-g")
