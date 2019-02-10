cmake_minimum_required(VERSION 3.0)

project(hashtable)

add_definitions("-Werror -Wall -std=c11 -Wextra -D_DEFAULT_SOURCE")

file(GLOB sources "src/hashtable.c")
file(GLOB headers "src/hashtable.h")

add_library(${PROJECT_NAME} SHARED ${headers} ${sources})
