include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)
if (WIN32)
	FetchContent_Declare(
		SDL2
		URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.9/SDL2-devel-2.30.9-mingw.zip)

	FetchContent_Declare(
		SDL2_image
		URL https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-devel-2.8.2-mingw.zip)

	FetchContent_Declare(
		SDL2_ttf
		URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-devel-2.22.0-mingw.zip)

	FetchContent_Declare(
		SDL2_mixer
		URL https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.0/SDL2_mixer-devel-2.8.0-mingw.zip)
	# Explicitly set SDL2 include directory and library path

	FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_ttf SDL2_mixer)
	list(APPEND CMAKE_PREFIX_PATH ${sdl2_SOURCE_DIR}/cmake)
	list(APPEND CMAKE_PREFIX_PATH ${sdl2_image_SOURCE_DIR}/cmake)
	list(APPEND CMAKE_PREFIX_PATH ${sdl2_mixer_SOURCE_DIR}/cmake)
	list(APPEND CMAKE_PREFIX_PATH ${sdl2_ttf_SOURCE_DIR}/cmake)
	message(STATUS "Checking for SDL2 library: ${sdl2_SOURCE_DIR}/lib/libSDL2.a")
	message(STATUS "Checking for SDL2 library: ${sdl2_image_SOURCE_DIR}/lib/libSDL2.a")
	message(STATUS "Checking for SDL2 library: ${sdl2_mixer_SOURCE_DIR}/lib/libSDL2.a")
	message(STATUS "Checking for SDL2 library: ${sdl2_ttf_SOURCE_DIR}/lib/libSDL2.a")
	find_package(SDL2 REQUIRED)
	find_package(SDL2_image REQUIRED)
	find_package(SDL2_ttf REQUIRED)
	find_package(SDL2_mixer REQUIRED)
else()
	FetchContent_Declare(
		SDL2
		URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.9/SDL2-2.30.9.zip)

	FetchContent_Declare(
		SDL2_image
		URL https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-2.8.2.zip)

	FetchContent_Declare(
		SDL2_ttf
		URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-2.22.0.zip)

	FetchContent_Declare(
		SDL2_mixer
		URL https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.0/SDL2_mixer-2.8.0.zip)
	
	FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_ttf SDL2_mixer)
endif()


