#include <beaver/text.hpp>

sdl::texture beaver::make_text_solid(SDL_Renderer* rdr,
		const sdl::font& font, 
		const std::string& content, 
		const std::array<unsigned char, 4> color,
		int wraplength)
{
	SDL_Color fg {color[0], color[1], color[2], color[3]};
	SDL_Surface* temp = TTF_RenderUTF8_Solid_Wrapped(font, content.c_str(), fg, wraplength);
	
	sdl::texture text {SDL_CreateTextureFromSurface(rdr, temp)};
	SDL_FreeSurface(temp);
	
	return text;
};
sdl::texture beaver::make_text_blended(SDL_Renderer* rdr,
		const sdl::font& font, 
		const std::string& content, 
		const std::array<unsigned char, 4> color,
		int wraplength)
{
	SDL_Color fg {color[0], color[1], color[2], color[3]};
	SDL_Surface* temp = TTF_RenderUTF8_Blended_Wrapped(font, content.c_str(), fg, wraplength);
	
	sdl::texture text {SDL_CreateTextureFromSurface(rdr, temp)};
	SDL_FreeSurface(temp);
	
	return text;
};
