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
	for (const auto& layer: tm._layerdata._layers)
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


sdl::texture sdl::render_text_solid(SDL_Renderer* rdr, 
		const sdl::font& font, 
		const std::string& content, 
		const std::array<unsigned char, 4>& color,
		int wraplength)
{
	SDL_Color fg {color[0], color[1], color[2], color[3]};
	SDL_Surface* temp;
	if (wraplength == 0) temp = TTF_RenderUTF8_Solid(font, content.c_str(), fg);
	else temp = TTF_RenderUTF8_Solid_Wrapped(font, content.c_str(), fg, wraplength);
	
	sdl::texture rs {SDL_CreateTextureFromSurface(rdr, temp)};
	SDL_FreeSurface(temp);
	return rs;
};
sdl::texture sdl::render_text_blended(SDL_Renderer* rdr,
		const sdl::font& font, 
		const std::string& content, 
		const std::array<unsigned char, 4>& color,
		int wraplength)
{
	SDL_Color fg {color[0], color[1], color[2], color[3]};
	SDL_Surface* temp;
	if (wraplength == 0) temp = TTF_RenderUTF8_Blended(font, content.c_str(), fg);
	else temp = TTF_RenderUTF8_Blended_Wrapped(font, content.c_str(), fg, wraplength);
	
	sdl::texture rs {SDL_CreateTextureFromSurface(rdr, temp)};
	SDL_FreeSurface(temp);
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

beaver::sdlgame::sdlgame()
{
	_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::io, sol::lib::table);

	_lua["ENGINE_PATH"] = ENGINE_PATH;
	_lua.script(R"(package.path = package.path .. ";" .. ENGINE_PATH .. "/utilities/luamodules/?.lua")");
	_lua.script(R"(salt = require("salt"))");

	// SDL
	int window_width {1280}, window_height {720}, FPS {60};
	std::string title {"NNM"};
	
	if (std::filesystem::exists("./config.lua"))
	{
		_lua.script(R"(config = require("config"))");
		window_width = _lua["config"]["window_width"];
		window_height = _lua["config"]["window_height"];
		title = _lua["config"]["title"];
	}
	else
	{
		_lua["config"] = _lua.create_table_with(
				"window_width", 1280,
				"window_height", 720,
				"title", "NNM"
				);
		_lua.script(R"(salt.save(config, "config.lua"))");
	};
	
	sdl::init_sdl(title.c_str(), window_width, window_height, _sdlwindow, _sdlrenderer);
	

	// Setup binding to Lua
	
	_lua.set_function("draw", 
			[&](const std::string& texture,
				const sol::table& src = {}, const sol::table& dst = {},
				const sol::table& rotation = {}, int flipflag = 0)
			{
				sdl::texture* tex = _assets.get<sdl::texture>(texture);
				SDL_Rect srcrect;
				if (src.valid())
				{
					if (
				};
				sdl::drawdata ddata {._tex = *tex,
									._src = {src["x"], src["y"], src["w"], src["h"]},
									._dst = {dst["x"], dst["y"], dst["w"], dst["h"]},
									._angle =
			})
										




#ifndef NDEBUG
	beaver::init_imgui(_sdlwindow, _sdlrenderer);
#endif
};

beaver::sdlgame::~sdlgame()
{	
	std::cout << "Quitting SDL\n";
	SDL_DestroyWindow(_sdlwindow);
	SDL_DestroyRenderer(_sdlrenderer);
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
void beaver::sdlgame::load_script()
{
	_lua.script_file("main.lua");
	//TODO make default init, update, draw and exit
};

void beaver::sdlgame::run()
{
	load_script();

	std::function<void()> initf = _lua["INIT"];
	std::function<bool(float)> updatef = _lua["UPDATE"];
	std::function<void()> drawf = _lua["DRAW"];
	std::function<void()> exitf = _lua["EXIT"];

	initf();

	_fpstracker.reset();
	
	SDL_Event sdlevent;
	bool loop_running {true};

	while (loop_running)
	{
		if (_fpstracker.new_frame_should_start())
		{
			_ctl.swap();
			while (SDL_PollEvent(&sdlevent))
			{
				ImGui_ImplSDL2_ProcessEvent(&sdlevent);
				_ctl.update(sdlevent);
				if (sdlevent.type == SDL_QUIT) 
				{
					//TODO: handle quit
					loop_running = false;
					_running = false;
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
			if (!updatef(_fpstracker.dt_ratio())) loop_running = false;
	
			drawf();

#ifndef NDEBUG
			ImGuiIO& io = ImGui::GetIO();
			SDL_RenderSetScale(_sdlrenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
			ImGui::Render();
			ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), _sdlrenderer);
#endif

        	SDL_RenderPresent(_sdlrenderer);
			
			_fpstracker.end_frame();
		};
	};

	exitf();
};

