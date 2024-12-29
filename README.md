# beaver
An efficient, lightweight 2D game engine built on SDL, featuring Lua bindings, an Entity Component System (ECS)

# Features

- SDL Integration: 
    - Graphics rendering (beaver::graphics)
        - Abstracting common drawing operations (point, line, rect, circle, image)
    - Asset management (beaver::assets_manager)
        - Based on templated class resource::manager
    - FPS tracking (beaver::FPS_tracker)
    - Input handling (beaver::controller)
    - RAII wrappers around SDL_Texture, Mix_Chunk, Mix_Music, TTF_Fonts
- Lua Scripting (with sol2): Scriptable game logic through seamless Lua bindings for fast iteration and customization.
- A tiny ECS with sensible default components: Modular and scalable architecture for game object management.
    Cross-Platform Support: Compatible with Linux, Windows, and potentially web builds using Emscripten.`

# How to use

- Put 'beaver' in your directory and `add_subdirectory(beaver_directory)`
- Link your game against `beaver` in your CMakeLists.txt
```
target_link_libraries(Game PRIVATE beaver)
```
