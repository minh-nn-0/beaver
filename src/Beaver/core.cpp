#include <Beaver/core.hpp>
#include <print>

void draw_layers(const tiled::layer& layer,
		tiled::drawdata	parent_drawdata,
		SDL_Renderer* rdr, 
		const beaver::camera2D& cam, 
		const tiled::tilemap& tm,
		const std::vector<sdl::texture*>& textures)
{
	if (tiled::layer_is_visible(layer))
	{
		if (std::holds_alternative<tiled::tilelayer>(layer))
		{
			const tiled::tilelayer& tl = std::get<tiled::tilelayer>(layer); 
			
			tiled::drawdata layer_drawdata = tl._drawdata + parent_drawdata;
			auto [opacity, parallax, offset, tint] = layer_drawdata;
		
			for (int i = 0; i != tl._data.size(); i++)
			{
				if (long tile = tl._data.at(i); tile >= 0)
				{
					auto [flipflag, tileid] = tiled::get_flipflags(tile);

					const tiled::tileset& ts = tm.tileset_at(tileid); 
					const sdl::texture* ts_tex = *(std::ranges::find_if(textures, 
								[=](auto&& tex){return ts._img == tex->_name;}));
					
					SDL_SetTextureAlphaMod(*ts_tex, opacity*255);
					SDL_SetTextureColorMod(*ts_tex, tint[0]*255, tint[1]*255, tint[2]*255);
					
					mmath::irect src(tiled::rect_at(tileid, ts));
					mmath::frect dst(tiled::rect_at(i, tm));
					
					// adding cam offset here is wrong
					dst._pos -= (cam._view._pos /* + cam._offset */ + offset) * parallax;
					
					//dst._pos.x -= cam._view._pos.x * parallax.x - offset.x;
					//dst._pos.y -= cam._view._pos.y * parallax.y - offset.y;
					
					sdl::draw(rdr, *ts_tex, src, dst, flipflag);
					
					SDL_SetTextureAlphaMod(*ts_tex, 255);
					SDL_SetTextureColorMod(*ts_tex, 255, 255, 255);
				};
			};
		}
		else if (std::holds_alternative<tiled::grouplayer>(layer)) 
			for (tiled::grouplayer group = std::get<tiled::grouplayer>(layer); 
					const auto grouplayer: group._layers) 
			{
				parent_drawdata += group._drawdata;
				draw_layers(grouplayer, parent_drawdata, rdr, cam, tm, textures);
			};
	};
};
void sdl::draw_tilemap(SDL_Renderer* rdr, 
		const beaver::camera2D& cam, 
		const tiled::tilemap& tm,
		const std::vector<sdl::texture*>& textures)
{
	for (const auto& layer: tm._layersdata)
		draw_layers(layer, {}, rdr, cam, tm, textures);
};


void sdl::draw_with_cam(const beaver::camera2D& cam, 
		SDL_Renderer* rdr, const sdl::drawdata& ddata)
{
	sdl::drawdata ddata_with_cam {ddata};
	// adding offset here is wrong, offset should be used only when call cam.target, like cam.target(target + 30) or something like that.
	ddata_with_cam._dst.x -= cam._view._pos.x /* + cam._offset.x */;
	ddata_with_cam._dst.y -= cam._view._pos.y /* + cam._offset.y */;

	sdl::draw(rdr, ddata_with_cam);

};

mmath::ivec2 sdl::render_output_size(const sdl::app& app)
{
	mmath::ivec2 rs;

	SDL_GetRendererOutputSize(app._renderer, &rs.x, &rs.y);

	return rs;
};
//void draw_tilelayer(SDL_Renderer* rdr,const tiled::tilelayer& tl, const tiled::tileset& ts)
//{
//	const auto& [opacity, parallax, offset, tint] = tl._drawdata;
//	for (int i = 0; i != tl._data.size(); i++)
//	{
//		if (int tile = tl._data[i]; tile >= 0)
//		{
//			//auto& ts = std::ranges::find_if(map._tilesets | std::views::reverse,[&](auto&& ts){return ts.first <= tile;})->second;
//			//SDL_SetTextureAlphaMod(ts._src, opacity*255);
//			
//			auto [flipflag, tileid] = tiled::get_flipflags(tile);
//			mg::frect dst = mg::rect_at(i, map);
//
//			dst._pos.x -= cam._view._pos.x * parallax.x - offset.x;
//			dst._pos.y -= cam._view._pos.y * parallax.y - offset.y;
//			
//			sdl.draw(ts._src, tiled::rect_at(tile, ts), dst, flipflag);
//
//			SDL_SetTextureAlphaMod(ts._src, 255);
//		}
//	}
//};
//	
//void drawlayers(const tiled::layer& l, SDL_Renderer* rdr) 
//{
//	
//	if (std::holds_alternative<tiled::tilelayer>(l))
//		draw_tilelayer(std::get<tiled::tilelayer>(l));
//
//	if (std::holds_alternative<tiled::grouplayer>(l))
//		for (auto& gr_l: std::get<tiled::grouplayer>(l)._layers)
//			drawlayers(gr_l, sdl);
//};

beaver::sdlgame::sdlgame(const std::string& title, int ww, int wh, int FPS)
	: _sdl(title.c_str(), ww, wh), _fpstracker(beaver::FPS_tracker{._FPS = FPS})
{
#ifndef NDEBUG
	beaver::init_imgui(*this);
#endif
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

void beaver::init_imgui(beaver::sdlgame& game)
{

#ifndef NDEBUG
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
    ImGui_ImplSDL2_InitForSDLRenderer(game._sdl._window, game._sdl._renderer);
    ImGui_ImplSDLRenderer2_Init(game._sdl._renderer);
#endif
};

void beaver::run_game_loop(beaver::sdlgame& game, 
							const std::function<bool(float)>& updatef, 
							const std::function<void()>& drawf)
{
	game._fpstracker.reset();

	SDL_Event sdlevent;

	bool loop_running {true};

	while (loop_running)
	{
		
		if (game._fpstracker.new_frame_should_start())
		{
			game._ctl.swap();
			while (SDL_PollEvent(&sdlevent))
			{
				ImGui_ImplSDL2_ProcessEvent(&sdlevent);
				game._ctl.update(sdlevent);
				if (sdlevent.type == SDL_QUIT) 
				{
					//TODO: handle quit
					loop_running = false;
					game._running = false;
				};
			};

#ifndef NDEBUG
	        // Start the Dear ImGui frame
			ImGui_ImplSDLRenderer2_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
#endif
			// Normally, dt_ratio() will return 1, if lag, return > 1, if too fast, return < 1
			// use dt_ratio in game logic is like "how many pixel a character move in one frame"
			if (!updatef(game._fpstracker.dt_ratio())) loop_running = false;
			drawf();

#ifndef NDEBUG
			ImGuiIO& io = ImGui::GetIO();
			SDL_RenderSetScale(game._sdl._renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
			ImGui::Render();
			ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), game._sdl._renderer);
#endif

        	SDL_RenderPresent(game._sdl._renderer);
			
			game._fpstracker.end_frame();
		};
	};

};

