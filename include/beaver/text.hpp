#pragma once

#include <sdlwrapper/sdlwrapper.hpp>

namespace beaver
{
	sdl::texture make_text_solid(SDL_Renderer* rdr, const sdl::font& font, const std::string& content, const std::array<unsigned char, 4> color, int wraplength = 0);
	sdl::texture make_text_shaded(SDL_Renderer* rdr, const sdl::font& font, const std::string& content,
			const std::array<unsigned char, 4> fg,
			const std::array<unsigned char, 4> bg,
			int wraplength = 0);
	sdl::texture make_text_blended(SDL_Renderer* rdr, const sdl::font& font, const std::string& content, const std::array<unsigned char, 4> color, int wraplength = 0);
};
