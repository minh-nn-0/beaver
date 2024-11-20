#ifndef BEAVER_GRAPHICS_H
#define BEAVER_GRAPHICS_H

#include <mmath/geometry.hpp>
#include <tiledwrapper/tiledwrapper.hpp>
#include <sdlwrapper/sdlwrapper.hpp>

namespace beaver
{
	namespace graphics
	{
		struct renderer
		{
		};
		namespace draw
		{
			void point(renderer&, const mmath::fvec2&);
			void line(renderer&, const mmath::fvec2&, const mmath::fvec2&);
			void rect(renderer&, const mmath::frect&);
			void circle(renderer&);
			void sprite(renderer&);
			void text(renderer&, const sdl::font&, const std::string&);
			void tilemap(renderer&, const tiled::tilemap&);
		};
	};
};
#endif
