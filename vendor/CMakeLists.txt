add_library(imgui_sdl SHARED
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

target_include_directories(beaver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headeronly)
