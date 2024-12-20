set(FREETYPE_LIBRARY ${CMAKE_CURRENT_LIST_DIR}/static_libs/freetype/libfreetype.a)
set(FREETYPE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/static_libs/freetype/include)

set(libxmp_LIBRARY ${CMAKE_CURRENT_LIST_DIR}/static_libs/libxmp/libxmp.a)
set(libxmp_INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/static_libs/libxmp/include)

# fluidsynth
set(FluidSynth_LIBRARY ${CMAKE_CURRENT_LIST_DIR}/static_libs/fluidsynth/libfluidsynth.a)
set(FluidSynth_INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/static_libs/fluidsynth/include)

# opusfile
set(OpusFile_LIBRARY ${CMAKE_CURRENT_LIST_DIR}/static_libs/opusfile/libopusfile.a)
set(OpusFile_INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/static_libs/opusfile/include)

# wavpack
set(wavpack_LIBRARY ${CMAKE_CURRENT_LIST_DIR}/static_libs/wavpack/libwavpack.a)
set(wavpack_INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/static_libs/wavpack/include)

add_subdirectory(SDL)

# IMGUI

add_library(imgui_sdl 
            ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl2.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdlrenderer2.cpp)

target_include_directories(imgui_sdl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends)

target_link_libraries(imgui_sdl PRIVATE sdlwrapper)

# LUA

add_subdirectory(lua)
# HEADERONLY
target_include_directories(beaver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headeronly)
