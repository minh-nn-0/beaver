#ifndef M_SDLLAYER_H
#define M_SDLLAYER_H

#include <SDL.h>
#include <SDL_mouse.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <print>
#include <filesystem>
namespace sdl
{
	void init_sdl(const char* title, int window_width, int window_height, SDL_Window** wd, SDL_Renderer** rdr);
	struct app
	{
		app(const char* title, int w, int h);

		~app();
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		
	};
	// ============= WRAPPER AROUND SDL STRUCTURE =================
	struct soundchunk
	{
		soundchunk() = default;
		soundchunk(const char* path): _chunk(Mix_LoadWAV(path)){	};
		soundchunk(Mix_Chunk* chunk): _chunk(chunk) {};
		soundchunk(soundchunk&& other)
		{
			_chunk = other._chunk;
			other._chunk = nullptr;
		};
		soundchunk& operator = (soundchunk&& other)
		{
			_chunk = other._chunk;
			other._chunk = nullptr;
			return *this;
		};
		~soundchunk()
		{
			if (_chunk != nullptr) Mix_FreeChunk(_chunk);
		};

		operator Mix_Chunk*() {return _chunk;};
	private:
		Mix_Chunk* _chunk;
	};

	struct music
	{
		music() = default;
		music(const char* path): _music(Mix_LoadMUS(path)){};
		music(Mix_Music* music): _music(music){};

		music(music&& other)
		{
			_music = other._music;
			other._music = nullptr;
		};
		music& operator = (music&& other)
		{
			_music = other._music;
			other._music = nullptr;
			return *this;
		};
		~music()
		{
			if (_music != nullptr) Mix_FreeMusic(_music);
		};
	
		operator Mix_Music*() {return _music;};
	private:
		Mix_Music* _music;
	};

	//Wrapper around SDL_Texture
	struct texture
	{
		texture() = default;
		texture(const std::string& path, SDL_Renderer* rdr) 
			: _name(std::filesystem::path(path).filename().string())	
		{
			SDL_Surface* surface = IMG_Load(path.c_str());
			_t = SDL_CreateTextureFromSurface(rdr, surface);
			SDL_FreeSurface(surface);
			SDL_QueryTexture(_t, NULL, NULL, &_width, &_height);
		};
		texture(SDL_Texture* t, const char* name = ""): _t(t), _name(name)
		{
			SDL_QueryTexture(_t, NULL, NULL, &_width, &_height);
		};
		texture(texture&& other)
		{
			_t = other._t;
			other._t = nullptr;
			_width = other._width;
			_height = other._height;
			_name = other._name;
		};
		texture& operator= (texture&& other)
		{
			_t = other._t;
			other._t = nullptr;
			_width = other._width;
			_height = other._height;
			_name = other._name;
			return *this;
		}
		~texture() 
		{
			if (_t) SDL_DestroyTexture(_t);
		};

		operator SDL_Texture*() const {return _t;};

		int _width, _height;
		std::string _name;

		SDL_Texture* _t;
	};

	//Wrapper around TTF_Font
	struct font
	{
		font() = default;
		font(const char* path, int fontsize): _font(TTF_OpenFont(path, fontsize)){};
		font(TTF_Font* f): _font{f}{};
		font(font&& other)
		{
			_font = other._font;
			other._font = nullptr;
		};
		font& operator = (font&& other)
		{
			_font = other._font;
			other._font = nullptr;
			return *this;
		};
		~font() 
		{
			if (_font != nullptr) TTF_CloseFont(_font);
		};
		operator TTF_Font*() const {return _font;};
	private:
		TTF_Font* _font;
	};

	// ================================================================================
	

	struct drawdata
	{
		SDL_Texture* _tex;
		SDL_Rect _src;
		SDL_FRect _dst;
		double _angle{0};
		SDL_FPoint _pivot;
		unsigned _flipflags {0};

	};

	void draw(SDL_Renderer*, const drawdata&);
	void draw(SDL_Renderer*,
			const sdl::texture&, const SDL_FRect& dst,
			unsigned flags = SDL_FLIP_NONE,
			double angle = 0, 
			const SDL_FPoint& pivot = {});

	void draw(SDL_Renderer*,
			const sdl::texture&, const SDL_Rect& src, const SDL_FRect& dst,  
			unsigned flags = SDL_FLIP_NONE,
			double angle = 0, 
			const SDL_FPoint& pivot = {});
};

#endif
