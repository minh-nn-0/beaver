#include <sdlwrapper/sdlwrapper.hpp>
#include <iostream>
sdl::app::app(const char* title, int w, int h)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	unsigned wflags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	unsigned rflags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
	_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w,h, wflags);
	if (!_window) std::cerr << "failed to create window" << SDL_GetError() << '\n';
	
	_renderer = SDL_CreateRenderer(_window, -1, rflags);
	if (!_renderer) std::cerr << "failed to create renderer" << SDL_GetError() << '\n';
	
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 1024);
	TTF_Init();
};


sdl::app::~app()
{
	std::cout << "Quitting SDL\n";
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
};

void sdl::draw(SDL_Renderer* rdr, const sdl::drawdata& ddata)
{
	const SDL_Rect* src = &ddata._src; 
	const SDL_FRect* dst = &ddata._dst;
	if (ddata._src.w == 0) src = nullptr;
	if (ddata._dst.w == 0) dst = nullptr;

	SDL_RenderCopyExF(rdr, ddata._tex, src, dst, 
			ddata._angle, &ddata._pivot, 
			static_cast<SDL_RendererFlip>(ddata._flipflags));	
};

void sdl::draw(SDL_Renderer* rdr, 
		const sdl::texture& tex, const SDL_FRect& dst,
		unsigned flags,
		double angle, 
		const SDL_FPoint& pivot) 
{
	const SDL_FRect* dst_ = dst.w == 0 ? nullptr : &dst;
	SDL_RenderCopyExF(rdr, tex, nullptr, dst_, 0, &pivot, static_cast<SDL_RendererFlip>(flags));
};

void sdl::draw(SDL_Renderer* rdr,
		const sdl::texture& tex, const SDL_Rect& src, const SDL_FRect& dst,
		unsigned flags,
		double angle, 
		const SDL_FPoint& pivot) 
{
	SDL_RenderCopyExF(rdr, tex, &src, &dst, 0, &pivot, static_cast<SDL_RendererFlip>(flags));
};
