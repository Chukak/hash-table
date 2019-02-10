cmake_minimum_required(VERSION 3.0)

project(hashtable)

add_definitions("-Werror -Wall -std=c11 -Wextra -D_DEFAULT_SOURCE")

file(GLOB sources "hashtable.c")
file(GLOB headers "hashtable.h")

add_library(${PROJECT_NAME} SHARED ${headers} ${sources})
