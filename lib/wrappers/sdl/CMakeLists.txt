cmake_minimum_required(VERSION 3.25)

project(sdlwrapper VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(sdlwrapper SHARED src/sdlwrapper.cpp)

target_include_directories(sdlwrapper PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
if (WIN32)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_image REQUIRED)
	find_package(SDL2_ttf REQUIRED)
	find_package(SDL2_mixer REQUIRED)
endif()

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
	target_link_libraries(sdlwrapper PUBLIC SDL2::SDL2main)
endif()

target_link_libraries(sdlwrapper PUBLIC SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)

add_subdirectory(test)
