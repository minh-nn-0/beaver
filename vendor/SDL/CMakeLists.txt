cmake_minimum_required(VERSION 3.25)

add_library(SDL2_layer INTERFACE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (WIN32)
	set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Windows/SDL2/SDL2-2.30.9/cmake)
	set(SDL2_image_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Windows/SDL2_image/SDL2_image-2.8.2/cmake)
	set(SDL2_ttf_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Windows/SDL2_ttf/SDL2_ttf-2.22.0/cmake)
	set(SDL2_mixer_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Windows/SDL2_mixer/SDL2_mixer-2.8.0/cmake)

	#	target_include_directories(SDL2_layer PUBLIC
	#		${SDL2_DIR}/x86_64-w64-mingw32/include
	#		${SDL2_image_DIR}/x86_64-w64-mingw32/include
	#		${SDL2_ttf_DIR}/x86_64-w64-mingw32/include
	#		${SDL2_mixer_DIR}/x86_64-w64-mingw32/include)
	#	target_link_libraries# 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
	find_package(SDL2 REQUIRED CONFIG)
## 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
#find_package(SDL2 REQUIRED CONFIG)

	find_package(SDL2_image REQUIRED CONFIG)
	find_package(SDL2_ttf REQUIRED CONFIG)
	find_package(SDL2_mixer REQUIRED CONFIG)

	target_include_directories(SDL2_layer INTERFACE
		${SDL2_DIR}/../include
		${SDL2_image_DIR}/../include
		${SDL2_ttf_DIR}/../include
		${SDL2_mixer_DIR}/../include)

else()
	set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Unix/SDL2/SDL2-2.30.9)
	set(SDL2_image_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Unix/SDL2_image/SDL2_image-2.8.2)
	set(SDL2_ttf_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Unix/SDL2_ttf/SDL2_ttf-2.22.0)
	set(SDL2_mixer_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Unix/SDL2_mixer/SDL2_mixer-2.8.0)
	
	add_subdirectory(${SDL2_DIR})
	add_subdirectory(${SDL2_image_DIR})
	add_subdirectory(${SDL2_ttf_DIR})
	add_subdirectory(${SDL2_mixer_DIR})
	
endif()
# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
	# It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
	target_link_libraries(SDL2_layer INTERFACE SDL2::SDL2main)
endif()

target_include_directories(SDL2_layer INTERFACE
	${SDL2_DIR}/include
	${SDL2_image_DIR}/include
	${SDL2_ttf_DIR}/include
	${SDL2_mixer_DIR}/include)
target_link_libraries(SDL2_layer INTERFACE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)


