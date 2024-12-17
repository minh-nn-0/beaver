#include <sdlwrapper/sdlwrapper.hpp>
#include <print>
#include <vector>
#include <algorithm>
#include <random>


struct particle
{
	SDL_Point _pos;
	int _lifetime;
};

int main()
{
	std::default_random_engine rde;
	std::uniform_int_distribution particle_dist {-10, 10};
	sdl::app test {"test1", 800, 600};
	auto [wdw, rdr] = test;
	bool running {true};
	SDL_Event e;
	SDL_Point mouse;

	SDL_RenderSetScale(rdr, 4,4);
	std::vector<particle> particles;
	while (running)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) running = false;
			if (e.type == SDL_MOUSEMOTION)
			{
				SDL_GetMouseState(&mouse.x, &mouse.y);
				particles.emplace_back(SDL_Point{mouse.x/4, mouse.y/4}, 100);
				std::println("{}, {}", mouse.x, mouse.y);
			}
		};
		
		SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
		SDL_RenderClear(rdr);
		
		SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
		
		for (const auto& p: particles)
		{
			SDL_Point pt {p._pos.x + particle_dist(rde), p._pos.y + particle_dist(rde)};
			SDL_RenderDrawPoint(rdr, pt.x, pt.y);
		};
		
		for (auto& p: particles) p._lifetime--;
		particles.erase(std::begin(std::ranges::remove_if(particles, 
										[](auto&& p) {return p._lifetime == 0;})),
						std::end(particles));
		SDL_RenderPresent(rdr);


	};
};
