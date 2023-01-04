cmake_minimum_required(VERSION 3.13)

add_library(bufio bufio.c)
target_include_directories(bufio PUBLIC include)