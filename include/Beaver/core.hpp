#ifndef BEAVER_CORE_H
#define BEAVER_CORE_H


#include <Beaver/input.hpp>
#include <Beaver/camera.hpp>
#include <Beaver/sprite.hpp>
//#include <Beaver/graphics.hpp>
#include <Beaver/tileanimation.hpp>
#include <Beaver/time.hpp>
#include <Beaver/event.hpp>
#include <Beaver/state.hpp>

#include <Beaver/entity.hpp>
#include <Beaver/resource.hpp>

#include <Beaver/AI.hpp>

#include <Beaver/enum_bitops.hpp>
#include <utilities.hpp>

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
};

namespace beaver
{
	constexpr unsigned MSPF_60 = 17; //milliseconds per frame
	constexpr unsigned MSPF_30 = 34; //milliseconds per frame
	struct sdlgame
	{
		using assets_manager = beaver::resource::manager<sdl::texture, sdl::soundchunk, sdl::music, sdl::font>;
		

		sdlgame(const std::string& title, int window_width, int window_height, int FPS = 60);
		
		bool 								_running {true};
		beaver::FPS_tracker 				_fpstracker;
		beaver::controller 					_ctl;
		sdl::app 							_sdl;
		assets_manager 						_assets;
		// ideally, will call once every 1000/fps (about 17 milliseconds for 60fps)
		std::function<bool(const float)> 	_updatef;
		std::function<void()> 				_drawf;
		
		void run();
	};
};
#endif
