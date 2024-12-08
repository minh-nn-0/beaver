#ifndef BEAVER_CORE_H
#define BEAVER_CORE_H


#include <Beaver/input.hpp>
#include <Beaver/camera.hpp>
#include <Beaver/sprite.hpp>
#include <Beaver/graphics.hpp>
#include <Beaver/tileanimation.hpp>
#include <Beaver/time.hpp>
#include <Beaver/event.hpp>
#include <Beaver/state.hpp>
#include <Beaver/entity.hpp>
#include <Beaver/resource.hpp>
#include <Beaver/AI.hpp>
#include <Beaver/enum_bitops.hpp>
#include <utilities.hpp>

//Scripting with Lua and Sol

#include <Beaver/scripting.hpp>

// wrappers
#include <sdlwrapper/sdlwrapper.hpp>
#include <tiledwrapper/tiledwrapper.hpp>


#ifndef NDEBUG
	//Imgui
	#include <imgui.h>
	#include <imgui_impl_sdl2.h>
	#include <imgui_impl_sdlrenderer2.h>
	#include <misc/cpp/imgui_stdlib.h>

#endif

namespace sdl
{
	void draw_tilemap(SDL_Renderer*, 
			const beaver::camera2D&, 
			const tiled::tilemap&, 
			const std::vector<sdl::texture*>&); 
	void draw_with_cam(const beaver::camera2D&,
			SDL_Renderer*, const sdl::drawdata&);

	mmath::ivec2 render_output_size(const sdl::app&);

	sdl::texture render_text_solid(SDL_Renderer*, 
			const sdl::font&, 
			const std::string&, 
			const std::array<unsigned char, 4>&, //color
			int wraplength = 0);
	sdl::texture render_text_blended(SDL_Renderer*,
			const sdl::font&, 
			const std::string&, 
			const std::array<unsigned char, 4>&,
			int wraplength = 0);
};

namespace beaver
{
	constexpr unsigned MSPF_60 = 17; //milliseconds per frame
	constexpr unsigned MSPF_30 = 34; //milliseconds per frame

	using assets_manager = beaver::resource::manager<sdl::texture, sdl::music, sdl::soundchunk, sdl::font>;

	struct sdlgame
	{
		sdlgame(const std::string& title, int window_width, int window_height);
		~sdlgame();
		bool 								_running {true};
		beaver::FPS_tracker 				_fpstracker;
		beaver::controller 					_ctl;
		beaver::graphics					_graphics;
		beaver::assets_manager				_assets;
		// SDL
	};
	void run_game(sdlgame&, const std::function<bool(float)>& updatef, const std::function<void()>& drawf);

	void init_imgui(SDL_Window* wd, SDL_Renderer* rdr);
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
