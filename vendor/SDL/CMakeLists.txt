include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
	SDL2
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-2.30.9
	GIT_PROGRESS TRUE
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2)

FetchContent_Declare(
	SDL2_image
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
	GIT_TAG release-2.8.2
	GIT_PROGRESS TRUE
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image)

FetchContent_Declare(
	SDL2_ttf
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
	GIT_TAG release-2.22.0
	GIT_PROGRESS TRUE
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf)

FetchContent_Declare(
	SDL2_mixer
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
	GIT_TAG release-2.8.0
	GIT_PROGRESS TRUE
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_mixer)

FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_ttf SDL2_mixer)

