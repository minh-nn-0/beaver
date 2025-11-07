#include <beaver/core.hpp>
#include <print>

#ifndef NDEBUG
void beaver::init_imgui(SDL_Window* wd, SDL_Renderer* rdr)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	// Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(wd, rdr);
    ImGui_ImplSDLRenderer2_Init(rdr);
};
void beaver::shutdown_imgui()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
};
#endif

beaver::sdlgame::sdlgame(const std::string& title, int window_width, int window_height)
{
	//_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::io, sol::lib::table);

	//_lua["ENGINE_PATH"] = ENGINE_PATH;
	//_lua.script(R"(package.path = package.path .. ";" .. ENGINE_PATH .. "/utilities/luamodules/?.lua")");
	//_lua.script(R"(salt = require("salt"))");

	// SDL
	sdl::init_sdl(title.c_str(), window_width, window_height, &_graphics._wd, &_graphics._rdr);

#ifndef NDEBUG
	beaver::init_imgui(_graphics._wd, _graphics._rdr);
#endif
};

beaver::sdlgame::~sdlgame()
{
	std::cout << "Quitting SDL\n";
#ifndef NDEBUG
	shutdown_imgui();
#endif
	_assets = {}; // cleanup assets
	std::println("Destroy window and renderer");
	SDL_DestroyWindow(_graphics._wd);
	SDL_DestroyRenderer(_graphics._rdr);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
};
//void beaver::sdlgame::run()
//{
//
//	_fpstracker.reset();
//	
//	SDL_Event e;
//	while (_running)
//	{
//
//	}
//};
void beaver::run_game(sdlgame& game, const std::function<bool(float)>& updatef, const std::function<void()>& drawf)
{
	game._fpstracker.reset();
	
	SDL_Event sdlevent;
	bool loop_running {true};

	while (loop_running)
	{
		if (game._fpstracker.new_frame_should_start())
		{
			float dt = game._fpstracker.elapsed_time_ms()/1000.f;

			game._gametime += dt;
#ifndef NDEBUG
			// Save current logical size
			int logicalW, logicalH;
			SDL_RenderGetLogicalSize(game._graphics._rdr, &logicalW, &logicalH);
			
			// TEMPORARILY DISABLE logical size for ImGui
			SDL_RenderSetLogicalSize(game._graphics._rdr, 0, 0);
			
			// Process SDL events normally
			while (SDL_PollEvent(&sdlevent))
			{
				ImGui_ImplSDL2_ProcessEvent(&sdlevent);
				game._ctl.update(sdlevent);
				if (sdlevent.type == SDL_QUIT) 
				{
					loop_running = false;
					game._running = false;
				}
			}
			
			// Start the Dear ImGui frame (now with logical size disabled)
			ImGui_ImplSDLRenderer2_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
			// RESTORE logical size before game rendering
			SDL_RenderSetLogicalSize(game._graphics._rdr, logicalW, logicalH);
#else
			while (SDL_PollEvent(&sdlevent))
			{
				game._ctl.update(sdlevent);
				if (sdlevent.type == SDL_QUIT) 
				{
					loop_running = false;
					game._running = false;
				}
			}
#endif
			if (!updatef(dt)) loop_running = false;
			
			
			drawf();
			
#ifndef NDEBUG
			// Disable logical size again for ImGui rendering
			SDL_RenderSetLogicalSize(game._graphics._rdr, 0, 0);
			
			ImGuiIO& io = ImGui::GetIO();
			ImGui::Render();
			ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), game._graphics._rdr);
			
			SDL_RenderSetLogicalSize(game._graphics._rdr, logicalW, logicalH);
#endif
			
			SDL_RenderPresent(game._graphics._rdr);
			
			for (auto& [_,v]: game._ctl._keystate)
				if (v > 0) v++;

			game._fpstracker.end_frame();
		};
	};

	std::println("exiting gameloop");
};

