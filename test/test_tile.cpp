#include <beaver/core.hpp>
int main(int argc, char** argv)
{
	beaver::tile::tilemap testmap (std::string(ENGINE_PATH) + "/test/test.tmj");

	beaver::sdlgame game {"testtile", 1280, 720};

	game._assets.add<sdl::texture>("tileset", 
			sdl::texture{std::string(ENGINE_PATH) + "/test/tileset.png", game._graphics._rdr});
	beaver::run_game(game, [&](float dt)
			{
				return true;
			},
			[&]()
			{
				game._graphics.set_draw_color({0,0,0,255});
				SDL_RenderClear(game._graphics._rdr);

				game._graphics.tilemap(testmap, {}, game._assets.get_vec<sdl::texture>());
			});

};
