#include <beaver/scripting/scripting_core.hpp>
void beaver::scripting::init_lua(sol::state& lua)
{
	lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::package, sol::lib::math, sol::lib::debug, sol::lib::string, sol::lib::io);
	lua.script((std::string("package.path = package.path .. \";") + std::string(ENGINE_PATH) + "/utilities/luamodules/?.lua\"").c_str());
};
void beaver::scripting::bind_core(beaver::sdlgame& game, sol::state& lua)
{
	// ASSETS
	lua.set_function("NEW_IMAGE", [&](const std::string& path, const std::string& custom_name)
			{
				std::string name = custom_name.empty() ? 
								std::filesystem::path(path).filename().string()
								: custom_name;
				game._assets.add<sdl::texture>(
						name,
						sdl::texture{path.c_str(), game._graphics._rdr});
			});
	
	lua.set_function("NEW_MUSIC", [&](const std::string& path, const std::string& custom_name)
			{
				std::string name = custom_name.empty() ? 
								std::filesystem::path(path).filename().string()
								: custom_name;
				game._assets.add<sdl::music>(
						name,
						sdl::music{path.c_str()});
			});
	lua.set_function("NEW_SOUND", [&](const std::string& path, const std::string& custom_name)
			{
				std::string name = custom_name.empty() ? 
								std::filesystem::path(path).filename().string()
								: custom_name;
				game._assets.add<sdl::soundchunk>(
						name,
						sdl::soundchunk{path.c_str()});
			});
	lua.set_function("NEW_FONT", [&](const std::string& path, int fontsize, const std::string& custom_name)
			{
				std::string name = custom_name.empty() ? 
								std::filesystem::path(path).filename().string()
								: custom_name;
				game._assets.add<sdl::font>(
						name,
						sdl::font{path.c_str(), fontsize});
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

	// RENDERING
	
	lua["FLIP_NONE"] = SDL_FLIP_NONE;
	lua["FLIP_H"] = SDL_FLIP_HORIZONTAL;
	lua["FLIP_V"] = SDL_FLIP_VERTICAL;
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
	lua.set_function("CREATE_TEXTURE_FOR_DRAWING", [&](const std::string& name, int width, int height)
			{
				game._assets.add<sdl::texture>(name, sdl::texture{SDL_CreateTexture(game._graphics._rdr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height)});
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
	lua.set_function("SET_TEXTURE_BLEND_MODE", [&](const std::string& texturename, const std::string& blendmode)
			{
				sdl::texture* tex = game._assets.get<sdl::texture>(texturename);
				if (blendmode == "additive") SDL_SetTextureBlendMode(*tex, SDL_BLENDMODE_ADD);
				if (blendmode == "modulate") SDL_SetTextureBlendMode(*tex, SDL_BLENDMODE_MOD);
				if (blendmode == "multiply") SDL_SetTextureBlendMode(*tex, SDL_BLENDMODE_MUL);
				if (blendmode == "blend") SDL_SetTextureBlendMode(*tex, SDL_BLENDMODE_BLEND);
			});
	lua.set_function("SET_RENDER_TARGET", [&](const std::string& name)
			{
				if (name.empty()) SDL_SetRenderTarget(game._graphics._rdr, nullptr);
				else SDL_SetRenderTarget(game._graphics._rdr, *game._assets.get<sdl::texture>(name));
			});

	lua.set_function("SET_FULLSCREEN", [&](bool fc){SDL_SetWindowFullscreen(game._graphics._wd, fc);});
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

	lua.set_function("DRAW_TEXTURE", [&](const std::string& texture_name, const sol::table& param)
			{
				double angle = param["angle"].get_or(0);
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
					pivot = {param["pivot"]["x"].get_or(0.f),
							 param["pivot"]["y"].get_or(0.f)};
				};
				
				sdl::texture* to_draw = game._assets.get<sdl::texture>(texture_name);
				game._graphics.texture(*to_draw, dst, src, angle, pivot, flipflag);
			});
	

	lua.set_function("SET_FONT_SIZE", [&](const std::string& fontname, int size)
			{
				sdl::font* font = game._assets.get<sdl::font>(fontname);
				TTF_SetFontSize(*font, size);
			});
	// Draw using topleft
	lua.set_function("DRAW_TEXT", [&](float x, float y, const std::string& fontname, float scale,
				const std::string& content, int wraplength, bool blended)
			{
				sdl::font* font = game._assets.get<sdl::font>(fontname);
				if (blended) 
					game._graphics.text_blended({x,y}, *font, content, scale, wraplength);
				else 
					game._graphics.text_solid({x,y}, *font, content, scale, wraplength);
			});
	// draw using center
	lua.set_function("DRAW_TEXT_CENTERED", [&](float x, float y, const std::string& fontname, float scale,
				const std::string& content, int wraplength, bool blended)
			{
				sdl::font* font = game._assets.get<sdl::font>(fontname);
				if (blended) 
					game._graphics.text_blended({x,y}, *font, content, scale, wraplength, graphics::TEXT_ALIGNMENT::CENTER);
				else 
					game._graphics.text_solid({x,y}, *font, content, scale, wraplength, graphics::TEXT_ALIGNMENT::CENTER);
			});
	lua.set_function("DRAW_TEXT_RIGHT", [&](float x, float y, const std::string& fontname, float scale,
				const std::string& content, int wraplength, bool blended)
			{
				sdl::font* font = game._assets.get<sdl::font>(fontname);
				if (blended) 
					game._graphics.text_blended({x,y}, *font, content, scale, wraplength, graphics::TEXT_ALIGNMENT::RIGHT);
				else 
					game._graphics.text_solid({x,y}, *font, content, scale, wraplength, graphics::TEXT_ALIGNMENT::RIGHT);
			});
	lua.set_function("IMAGE_SIZE", [&](const std::string& name) -> sol::table
			{
				if (!game._assets.get_map<sdl::texture>().contains(name))
					return sol::nil;
				return lua.create_table_with(
						"w", game._assets.get<sdl::texture>(name)->_width,
						"h", game._assets.get<sdl::texture>(name)->_height);
			});


	lua.set_function("PLAY_SOUND", [&](const std::string& name, int channel, int loop)
			{
				sdl::soundchunk* sound = game._assets.get<sdl::soundchunk>(name);

				Mix_PlayChannel(channel, *sound, loop);
			});
	lua.set_function("PLAY_MUSIC", [&](const std::string& name, int loop)
			{
				sdl::music* music = game._assets.get<sdl::music>(name);

				Mix_PlayMusic(*music, loop);
			});

	lua.set_function("SET_VOLUME_MASTER", [&](int volume)
			{
				Mix_MasterVolume(volume);
			});
	lua.set_function("SET_VOLUME_MUSIC", [&](int volume)
			{
				Mix_VolumeMusic(volume);
			});
	lua.set_function("SET_VOLUME_SOUND", [&](const std::string& soundname, int volume)
			{
				sdl::soundchunk* sound = game._assets.get<sdl::soundchunk>(soundname);
				Mix_VolumeChunk(*sound, volume);
			});
	lua.set_function("SET_VOLUME_CHANNEL", [&](int channel, int volume)
			{
				Mix_Volume(channel, volume);
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
