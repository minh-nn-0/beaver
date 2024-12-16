#include <beaver/scripting.hpp>
void beaver::init_lua(sol::state& lua)
{
	lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::package, sol::lib::math, sol::lib::debug, sol::lib::string);
	lua.script((std::string("package.path = package.path .. \";") + std::string(ENGINE_PATH) + "/utilities/luamodules/?.lua\"").c_str());
};
void beaver::bind_core(beaver::sdlgame& game, sol::state& lua)
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
	lua.set_function("SET_DRAW_COLOR", 
			[&](unsigned char r, unsigned char g, unsigned char b, unsigned char a)
			{game._graphics.set_draw_color({r,g,b,a});});

	lua.set_function("SET_SCALE", [&](int x, int y)
			{SDL_RenderSetScale(game._graphics._rdr, x, y);});

	lua.set_function("SET_RENDER_LOGICAL_SIZE", [&](int x, int y)
			{SDL_RenderSetLogicalSize(game._graphics._rdr, x, y);});

	lua.set_function("GET_RENDER_LOGICAL_SIZE", [&]() -> sol::table
			{
				int x, y;
				SDL_RenderGetLogicalSize(game._graphics._rdr, &x, &y);
				return lua.create_table_with("x", x, "y", y);
			});
	lua.set_function("SET_FULLSCREEN", [&](bool fc){SDL_SetWindowFullscreen(game._graphics._wd, fc);});
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

	lua.set_function("DRAW_TEXT", [&](float x, float y, const std::string& fontname, const std::string& content, int wraplength, bool blended)
			{
				sdl::font* font = game._assets.get<sdl::font>(fontname);
				if (blended) 
					game._graphics.text_blended({x,y}, *font, content, wraplength);
				else 
					game._graphics.text_solid({x,y}, *font, content, wraplength);
			});
	// 
	
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
