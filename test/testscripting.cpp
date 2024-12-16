#include <Beaver/core.hpp>


int main()
{
	beaver::sdlgame game {"testscript", 800, 600};
	
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::package);

	beaver::setup_core_binding(game, lua);

	lua.script_file((std::string(ENGINE_PATH) + "/test/testscript.lua").c_str());
	
	lua["LOAD"]();
	sol::function drawf = lua["DRAW"];
	bool running = true;
	while (running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) running = false;
		};
		SDL_SetRenderDrawColor(game._graphics._rdr,100,20,30,255);
		SDL_RenderClear(game._graphics._rdr);

		SDL_RenderSetScale(game._graphics._rdr, 5,5);
		SDL_SetRenderDrawColor(game._graphics._rdr, 255,255,255,255);

		drawf();
		SDL_RenderPresent(game._graphics._rdr);
	}


	

};
