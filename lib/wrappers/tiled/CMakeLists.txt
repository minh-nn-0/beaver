cmake_minimum_required(VERSION 3.25)

project(tiledwrapper VERSION 1.0 LANGUAGES CXX)

add_library(tiledwrapper STATIC src/tiledwrapper.cpp)
target_include_directories(tiledwrapper PUBLIC include ${VENDOR_DIR} ${UTILITIES_DIR})
target_link_libraries(tiledwrapper PRIVATE mmath)

