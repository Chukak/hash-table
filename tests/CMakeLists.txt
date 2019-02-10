cmake_minimum_required(VERSION 3.0)

project(tests_hashtable)

add_definitions("-std=c11 -Wextra -D_DEFAULT_SOURCE")

include_directories("../src/")
file(GLOB sources "../src/hashtable.c")
file(GLOB headers "../src/hashtable.h")

file(GLOB_RECURSE sources_tests "*.c")
file(GLOB_RECURSE headers_tests "*.h")

add_executable(${PROJECT_NAME} ${sources} ${headers}
    ${headers_tests} ${sources_tests})
