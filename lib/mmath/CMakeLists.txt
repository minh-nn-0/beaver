cmake_minimum_required(VERSION 3.25)

project(math VERSION 1.0)

add_library(mmath INTERFACE)
target_include_directories(mmath INTERFACE include)
