#include <beaver/scripting/scripting_core.hpp>
void beaver::scripting::init_lua(sol::state& lua)
{
	lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::package, sol::lib::math, sol::lib::debug, sol::lib::string, sol::lib::io);
	lua.script((std::string("package.path = package.path .. \";") + std::string(ENGINE_PATH) + "/utilities/luamodules/?.lua\"").c_str());
};
void check_path(const std::string& path)
{
	if (!std::filesystem::exists(path)) throw std::runtime_error(std::format("path {} not found", path));
}
void beaver::scripting::bind_core(beaver::sdlgame& game, sol::state& lua)
{
	// ASSETS
	lua.set_function("NEW_IMAGE", [&](const std::string& path)
			{
				check_path(path);
				auto& vec = game._assets.get_vec<sdl::texture>();
				vec.emplace_back(path.c_str(), game._graphics._rdr);
				return vec.size() - 1;
			});
	
	lua.set_function("NEW_MUSIC", [&](const std::string& path)
			{
				check_path(path);
				auto& vec = game._assets.get_vec<sdl::music>();
				vec.emplace_back(path.c_str());
				return vec.size() - 1;
			});
	lua.set_function("NEW_SOUND", [&](const std::string& path)
			{
				check_path(path);
				auto& vec = game._assets.get_vec<sdl::soundchunk>();
				vec.emplace_back(path.c_str());
				return vec.size() - 1;
			});
	lua.set_function("NEW_FONT", [&](const std::string& path, int fontsize)
			{
				check_path(path);
				auto& vec = game._assets.get_vec<sdl::font>();
				vec.emplace_back(path.c_str(), fontsize);
				return vec.size() - 1;
			});

	// FPS and Time
	lua.set_function("GET_ELAPSED_TIME", [&]() -> float
			{
				return game._gametime;
			});
	// INPUT
	lua.set_function("GET_INPUT", [&](const std::string& keyname) -> int
			{
				return game._ctl._keystate[KEYMAP.at(keyname)];
			});
    lua.set_function("GET_MOUSE", [&]() -> std::pair<int, int>
            {
                int x,y;
                SDL_GetMouseState(&x, &y);
                return std::make_pair(x,y);
            });
	// RENDERING
	
	lua["FLIP_NONE"] = SDL_FLIP_NONE;
	lua["FLIP_H"] = SDL_FLIP_HORIZONTAL;
	lua["FLIP_V"] = SDL_FLIP_VERTICAL;
	lua["TILED_FLIP_V"] = tiled::TILED_FLIPFLAG_V;
	lua["TILED_FLIP_H"] = tiled::TILED_FLIPFLAG_H;
	lua.set_function("CLS", [&]{ SDL_RenderClear(game._graphics._rdr);});
	lua.set_function("CLS", [&]{ SDL_RenderClear(game._graphics._rdr);});
	lua.set_function("SET_DRAW_COLOR", 
			[&](unsigned char r, unsigned char g, unsigned char b, unsigned char a)
			{game._graphics.set_draw_color({r,g,b,a});});

	lua.set_function("SET_SCALE", [&](int x, int y)
			{SDL_RenderSetScale(game._graphics._rdr, x, y);});
	lua.set_function("SET_INTEGER_SCALE", [&](bool active)
			{
				SDL_RenderSetIntegerScale(game._graphics._rdr, active ? SDL_TRUE : SDL_FALSE);
			});
	lua.set_function("SET_RENDER_LOGICAL_SIZE", [&](int x, int y)
			{SDL_RenderSetLogicalSize(game._graphics._rdr, x, y);});

	lua.set_function("GET_RENDER_LOGICAL_SIZE", [&]() -> std::pair<int,int>
			{
				int x, y;
				SDL_RenderGetLogicalSize(game._graphics._rdr, &x, &y);
				return std::make_pair(x,y);
			});
	lua.set_function("GET_RENDER_OUTPUT_SIZE", [&]() -> std::pair<int,int>
			{
				int x, y;
				SDL_GetRendererOutputSize(game._graphics._rdr, &x, &y);
				return std::make_pair(x,y);
			});
	lua.set_function("CREATE_TEXTURE_FOR_DRAWING", [&](int width, int height)
			{
				auto& vec = game._assets.get_vec<sdl::texture>();
				vec.emplace_back(SDL_CreateTexture(game._graphics._rdr, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height));
				return vec.size() - 1;
			});
	lua.set_function("SET_VIEWPORT", [&](int x, int y, int w, int h)
			{
				SDL_Rect vp = {x,y,w,h};
				SDL_RenderSetViewport(game._graphics._rdr, &vp);
			});
	lua.set_function("RESET_VIEWPORT", [&]
			{
				SDL_RenderSetViewport(game._graphics._rdr, nullptr);
			});
	lua.set_function("SET_CLIP_RECT", [&](int x, int y, int w, int h)
			{
				SDL_Rect clip = {x,y,w,h};
				SDL_RenderSetClipRect(game._graphics._rdr, &clip);
			});
	lua.set_function("RESET_CLIP_RECT", [&]
			{
				SDL_RenderSetClipRect(game._graphics._rdr, nullptr);
			});
	lua.set_function("SET_TEXTURE_BLEND_MODE", [&](std::size_t index, const std::string& blendmode)
			{
				auto& tex = game._assets.get_vec<sdl::texture>().at(index);
				if (blendmode == "additive") SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_ADD);
				if (blendmode == "modulate") SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_MOD);
				if (blendmode == "multiply") SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_MUL);
				if (blendmode == "blend") SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
			});
	lua.set_function("SET_TEXTURE_COLOR_MOD", [&](std::size_t index, const sol::table& color)
			{
				auto& tex = game._assets.get_vec<sdl::texture>().at(index);
				SDL_SetTextureColorMod(tex, color[1], color[2], color[3]);
				SDL_SetTextureAlphaMod(tex, color[4]);
			});
	lua.set_function("SET_RENDER_BLEND_MODE", [&](const std::string& blendmode)
			{
				if (blendmode == "additive") SDL_SetRenderDrawBlendMode(game._graphics._rdr, SDL_BLENDMODE_ADD);
				if (blendmode == "modulate") SDL_SetRenderDrawBlendMode(game._graphics._rdr, SDL_BLENDMODE_MOD);
				if (blendmode == "multiply") SDL_SetRenderDrawBlendMode(game._graphics._rdr, SDL_BLENDMODE_MUL);
				if (blendmode == "blend") SDL_SetRenderDrawBlendMode(game._graphics._rdr, SDL_BLENDMODE_BLEND);
			});
	lua.set_function("SET_RENDER_TARGET", [&](std::size_t textureid)
			{
				if (textureid == -1)
#ifdef USE_VIRTUAL_RENDER_TARGET
					SDL_SetRenderTarget(game._graphics._rdr, game._graphics._vrendertarget);
#else
					SDL_SetRenderTarget(game._graphics._rdr, nullptr);
#endif
				else
					SDL_SetRenderTarget(game._graphics._rdr, game._assets.get_vec<sdl::texture>().at(textureid));
			});

	lua.set_function("SET_FULLSCREEN", [&](bool fc){SDL_SetWindowFullscreen(game._graphics._wd, fc ? SDL_WINDOW_FULLSCREEN_DESKTOP : fc);});
	lua.set_function("SET_USING_CAM", [&](bool usingcam){game._graphics.set_cam(usingcam);});
	lua.set_function("DRAW_POINT", [&](float x, float y)
			{
				game._graphics.point(x,y);	
			});
	lua.set_function("DRAW_LINE", [&](float x1, float y1, float x2, float y2)
			{
				game._graphics.line(x1,y1,x2,y2);
			});
	lua.set_function("DRAW_RECTANGLE", [&](float x, float y, float width, float height, bool filled)
			{
				game._graphics.rect(x,y,width,height,filled);
			});
	lua.set_function("DRAW_CIRCLE", [&](float x, float y, float radius, bool filled)
			{
				game._graphics.circle(x,y,radius,filled);
			});

	lua.set_function("DRAW_TEXTURE", [&](std::size_t index, const sol::table& param)
			{
				auto& tex = game._assets.get_vec<sdl::texture>().at(index);
				double angle = param["angle"].get_or(0.f);
				unsigned flipflag = param["flipflag"].get_or(0);
				
				mmath::frect dst {0,0,0,0}, src {0,0,0,0};
				mmath::fvec2 pivot {0,0};
				if (param["dst"].valid())
				{
					auto luadst = param["dst"];
					dst =  { luadst["x"].get_or(0.f),
							 luadst["y"].get_or(0.f),
							 luadst["w"].get_or(0.f),
							 luadst["h"].get_or(0.f)
							};
				};

				if (param["src"].valid())
				{
					auto luasrc = param["src"];
					src =  { luasrc["x"].get_or(0.f),
							 luasrc["y"].get_or(0.f),
							 luasrc["w"].get_or(0.f),
							 luasrc["h"].get_or(0.f)
							};
				};

				if (param["pivot"].valid())
				{
					pivot = {param["pivot"][1].get_or(0.f),
							 param["pivot"][2].get_or(0.f)};
				};
				game._graphics.texture(tex, dst, src, angle, pivot, flipflag);
			});
	

	lua.set_function("SET_FONT_SIZE", [&](std::size_t fontid, int size)
			{
				auto& font = game._assets.get_vec<sdl::font>().at(fontid);
				TTF_SetFontSize(&*font, size);
			});
	lua.set_function("SET_FONT_OUTLINE", [&](std::size_t fontid, int size)
			{
				auto& font = game._assets.get_vec<sdl::font>().at(fontid);
				TTF_SetFontOutline(&*font, size);
			});
	lua.set_function("SET_FONT_STYLE", [&](std::size_t fontid, int style)
			{
				auto& font = game._assets.get_vec<sdl::font>().at(fontid);
				TTF_SetFontStyle(&*font, style);
			});
	// Draw using topleft
	lua.set_function("DRAW_TEXT", [&](float x, float y, std::size_t fontid, float scale,
				const std::string& content, int wraplength, bool blended)
			{
				auto& font = game._assets.get_vec<sdl::font>().at(fontid);
				if (blended) 
					game._graphics.text_blended({x,y}, font, content, scale, wraplength);
				else 
					game._graphics.text_solid({x,y}, font, content, scale, wraplength);
			});
	// draw using center
	lua.set_function("DRAW_TEXT_CENTERED", [&](float x, float y, std::size_t fontid, float scale,
				const std::string& content, int wraplength, bool blended)
			{
				auto& font = game._assets.get_vec<sdl::font>().at(fontid);
				if (blended) 
					game._graphics.text_blended({x,y}, font, content, scale, wraplength, graphics::TEXT_ALIGNMENT::CENTER);
				else 
					game._graphics.text_solid({x,y}, font, content, scale, wraplength, graphics::TEXT_ALIGNMENT::CENTER);
			});
	lua.set_function("DRAW_TEXT_RIGHT", [&](float x, float y, std::size_t fontid, float scale,
				const std::string& content, int wraplength, bool blended)
			{
				auto& font = game._assets.get_vec<sdl::font>().at(fontid);
				if (blended) 
					game._graphics.text_blended({x,y}, font, content, scale, wraplength, graphics::TEXT_ALIGNMENT::RIGHT);
				else 
					game._graphics.text_solid({x,y}, font, content, scale, wraplength, graphics::TEXT_ALIGNMENT::RIGHT);
			});
	lua.set_function("IMAGE_SIZE", [&](std::size_t img_id)
			{
				const auto& tex = game._assets.get_vec<sdl::texture>().at(img_id);
				return std::make_pair(tex._width, tex._height);
			});


	lua.set_function("PLAY_SOUND", [&](std::size_t soundid, int channel, int loop)
			{
				return Mix_PlayChannel(channel, game._assets.get_vec<sdl::soundchunk>().at(soundid), loop);
			});
	lua.set_function("PAUSE_CHANNEL", [&](int channel)
			{
				Mix_Pause(channel);
			});
	lua.set_function("HALT_CHANNEL", [&](int channel)
			{
				Mix_HaltChannel(channel);
			});
	lua.set_function("ALLOCATE_CHANNELS", [&](int number)
			{
				Mix_AllocateChannels(number);
			});
	lua.set_function("FADE_IN_MUSIC", [&](std::size_t musicid, int loop, int ms)
			{
				Mix_FadeInMusic(game._assets.get_vec<sdl::music>().at(musicid), loop, ms);
			});
	lua.set_function("FADE_OUT_MUSIC", [&](int ms)
			{
				Mix_FadeOutMusic(ms);
			});
	lua.set_function("FADE_IN_CHANNEL", [&](std::size_t soundid, int channel, int loop, int ms)
			{
				return Mix_FadeInChannel(channel, game._assets.get_vec<sdl::soundchunk>().at(soundid), loop, ms);
			});
	lua.set_function("FADE_OUT_CHANNEL", [&](int channel, int ms)
			{
				Mix_FadeOutChannel(channel, ms);
			});
	lua.set_function("CHANNEL_PLAYING", [&](int channel)
			{
				return Mix_Playing(channel) == 1 ? true:false;
			});
	lua.set_function("MUSIC_PLAYING", [&]()
			{
				return Mix_PlayingMusic() == 1 ? true:false;
			});
	lua.set_function("PLAY_MUSIC", [&](std::size_t musicid, int loop)
			{
				Mix_PlayMusic(game._assets.get_vec<sdl::music>().at(musicid), loop);
			});
	lua.set_function("SET_VSYNC", [&](bool on)
			{
				SDL_RenderSetVSync(game._graphics._rdr, on ? 1 : 0);
			});
	lua.set_function("PAUSE_MUSIC", [&]()
			{
				Mix_PauseMusic();
			});
	lua.set_function("SET_VOLUME_MASTER", [&](int volume)
			{
				Mix_MasterVolume(volume);
			});
	lua.set_function("SET_VOLUME_MUSIC", [&](int volume)
			{
				Mix_VolumeMusic(volume);
			});
	lua.set_function("SET_VOLUME_SOUND", [&](std::size_t soundid, int volume)
			{
				Mix_VolumeChunk(game._assets.get_vec<sdl::soundchunk>().at(soundid), volume);
			});
	lua.set_function("SET_VOLUME_CHANNEL", [&](int channel, int volume)
			{
				Mix_Volume(channel, volume);
			});
	lua.set_function("SET_PANNING", [&](int channel, int left, int right)
			{
				Mix_SetPanning(channel, left, right);
			});
};


//void beaver::bind_tiled(beaver::sdlgame& game, sol::table& tbl, sol::state& lua)
//{
//	tbl.set_function("add_map");
//	tbl.set_function("draw_map");
//	tbl.set_function("");
//	tbl.set_function("add_map");
//	tbl.set_function("add_map");
//	tbl.set_function("add_map");
//	tbl.set_function("add_map");
//};
