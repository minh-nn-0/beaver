cmake_minimum_required(VERSION 3.25)

project(beaver
	VERSION 1.0
	DESCRIPTION "A game engine based on SDL"
	LANGUAGES CXX
)
enable_testing()
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BEAVER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/beaver)
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(UTILITIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utilities)

add_library(beaver SHARED 
	${BEAVER_SRC_DIR}/core.cpp
	${BEAVER_SRC_DIR}/graphics.cpp
	${BEAVER_SRC_DIR}/scripting.cpp
	${BEAVER_SRC_DIR}/ecs/systems/particle.cpp
)
target_include_directories(beaver PUBLIC 
				${CMAKE_CURRENT_SOURCE_DIR}/include 
				${UTILITIES_DIR})

target_compile_definitions(beaver PUBLIC ENGINE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(lib)
add_subdirectory(${VENDOR_DIR})

target_link_libraries(beaver PUBLIC sdlwrapper imgui_sdl mmath tiledwrapper lua)

add_subdirectory(test EXCLUDE_FROM_ALL)
