include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)
if (WIN32)
	FetchContent_Declare(
		SDL2
		URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.9/SDL2-devel-2.30.9-mingw.zip
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2)

	FetchContent_Declare(
		SDL2_image
		URL https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-devel-2.8.2-mingw.zip
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image)

	FetchContent_Declare(
		SDL2_ttf
		URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-devel-2.22.0-mingw.zip
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf)

	FetchContent_Declare(
		SDL2_mixer
		URL https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.0/SDL2_mixer-2.8.0.zip
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_mixer)
	find_package(SDL2 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/cmake NO_DEFAULT_PATH)
	find_package(SDL2_image PATHS ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image/cmake NO_DEFAULT_PATH)
	find_package(SDL2_ttf PATHS ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf/cmake NO_DEFAULT_PATH)
	find_package(SDL2_mixer PATHS ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_mixer/cmake NO_DEFAULT_PATH)
else()
	FetchContent_Declare(
		SDL2
		URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.9/SDL2-2.30.9.zip
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2)

	FetchContent_Declare(
		SDL2_image
		URL https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-2.8.2.zip
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image)

	FetchContent_Declare(
		SDL2_ttf
		URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-2.22.0.zip
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf)

	FetchContent_Declare(
		SDL2_mixer
		URL https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.0/SDL2_mixer-2.8.0.zip
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_mixer)
	
	FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_ttf SDL2_mixer)
endif()


