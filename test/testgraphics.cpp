#include <Beaver/graphics.hpp>


int main()
{
	beaver::graphics graphics;
	sdl::init_sdl("hihi", 1280, 720, &graphics._wd, &graphics._rdr);
	
	bool running = true;
	sdl::texture test_image("/home/minhmacg/projects/videogames/madewithlove/space_shooter/assets/spritesheet.png",graphics._rdr);
	while (running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) running = false;
		};
		SDL_SetRenderDrawColor(graphics._rdr,100,20,30,255);
		SDL_RenderClear(graphics._rdr);

		SDL_RenderSetScale(graphics._rdr, 5,5);
		SDL_SetRenderDrawColor(graphics._rdr, 255,255,255,255);
		graphics.line(10,10,50,50);
		graphics.point(59,151);
		graphics.rect(20,50,200,400,true);
		graphics.circle(120,230,6, true);
		graphics.circle({30,40,5}, true);
		graphics.circle({40,20,10}, false);
		graphics.texture(test_image);
		graphics.texture(test_image, {30,40,50,50});
		graphics.texture(test_image, {}, {0,0,16,16});

		SDL_RenderPresent(graphics._rdr);
	}

	SDL_DestroyWindow(graphics._wd);
	SDL_DestroyRenderer(graphics._rdr);
};
