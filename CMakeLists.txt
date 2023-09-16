cmake_minimum_required(VERSION 3.25)
project(GZIP_V1 C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(GZIP_V1 main.c include/common.c include/input_process.c)
