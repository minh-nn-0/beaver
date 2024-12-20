#include "/home/minhmacg/.cache/emscripten/sysroot/include/emscripten.h"

#include <beaver/core.hpp>

beaver::sdlgame game {"testems", 1280, 400};

SDL_Event sdlevent;
float x = 50, y = 50;
sdl::texture testtex {"endlessrunner_tileset.png", game._graphics._rdr};
void draw()
{
	game._gametime += 1/60;
	while (SDL_PollEvent(&sdlevent))
	{
		game._ctl.update(sdlevent);
		if (sdlevent.type == SDL_QUIT) 
		{
			//TODO: handle quit
			game._running = false;
		};
	};

	std::println("hihihihi");
	if (game._ctl._keystate[SDLK_LEFT] > 0) x-= 10;
	if (game._ctl._keystate[SDLK_RIGHT] > 0) x+= 10;

	SDL_SetRenderDrawColor(game._graphics._rdr, 255,255,255,255);

	SDL_RenderClear(game._graphics._rdr);
	SDL_Surface* surface = IMG_Load("endlessrunner_tileset.png");
	if (!surface) {
		std::cerr << "Error loading texture: " << IMG_GetError() << std::endl;
		return;
	}

	SDL_FreeSurface(surface);
	SDL_SetRenderDrawColor(game._graphics._rdr, 0, 0, 0,255);
	game._graphics.texture(testtex);

	SDL_RenderPresent(game._graphics._rdr);
};
int main(int argc, char** argv)
{
	emscripten_set_main_loop(draw, 60,1);	
};

