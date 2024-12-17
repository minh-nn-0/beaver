#ifndef BEAVER_SDLGAME_H
#define BEAVER_SDLGAME_H

#include <beaver/FPS.hpp>
#include <beaver/graphics.hpp>
#include <beaver/input.hpp>
#include <beaver/resource.hpp>

namespace beaver
{
	constexpr unsigned MSPF_60 = 17; //milliseconds per frame
	constexpr unsigned MSPF_30 = 34; //milliseconds per frame

	using assets_manager = beaver::resource::manager<sdl::texture, sdl::music, sdl::soundchunk, sdl::font>;

	struct sdlgame
	{
	private:
		SDL_Window* _wd;
		SDL_Renderer* _rdr;
	public:
		sdlgame(const std::string& title, int window_width, int window_height);
		~sdlgame();
		bool 								_running {true};
		float								_gametime;
		beaver::graphics					_graphics;
		beaver::assets_manager				_assets;
		beaver::FPS_tracker 				_fpstracker;
		beaver::controller 					_ctl;

		mmath::ivec2 render_size()
		{
			mmath::ivec2 rs;
			SDL_RenderGetLogicalSize(_graphics._rdr, &rs.x, &rs.y);
			return rs;
		};

		mmath::ivec2 window_size()
		{
			mmath::ivec2 rs;
			SDL_GetWindowSize(_graphics._wd, &rs.x, &rs.y);
			return rs;
		};
		// SDL
	};
	void run_game(sdlgame&, const std::function<bool(float)>& updatef, const std::function<void()>& drawf);

	void init_imgui(SDL_Window* wd, SDL_Renderer* rdr);
	void shutdown_imgui();
	//struct game
	//{
	//private:
	//	sdl::app _sdl;
	//	bool _running;
	//	std::queue<sdl::drawdata> _drawqueue;
	//public:
	//	beaver::FPS_tracker _fpstracker;
	//	beaver::controller _ctl;
	//	void draw();
	//	void run_loop(const gameloop&);
	//};

	
	//void run_game_loop(sdlgame&, const gameloop&);
	
};

#endif
