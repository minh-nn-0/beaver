#ifndef BEAVER_GRAPHICS_H
#define BEAVER_GRAPHICS_H

#include <mmath/geometry.hpp>
#include <tiledwrapper/tiledwrapper.hpp>
#include <sdlwrapper/sdlwrapper.hpp>
#include <beaver/camera.hpp>
#include <beaver/tile.hpp>
#include <beaver/text.hpp>
namespace beaver
{
	constexpr mmath::fvec2 position_with_cam(const mmath::fvec2& pos, const beaver::camera2D& cam)
	{
		return (pos - cam._view._pos + cam._offset) * cam._zoom;
	};

	struct graphics
	{
		graphics() = default;
		graphics(SDL_Window*, SDL_Renderer*, camera2D* = nullptr);
		SDL_Window* _wd;
		SDL_Renderer* _rdr;
#ifdef USE_VIRTUAL_RENDER_TARGET
		SDL_Texture* _vrendertarget = nullptr;
#endif
		
		camera2D* _cam {nullptr};

		bool _using_cam {true};
		using color = std::array<unsigned char, 4>;
		color _draw_color;
		int _line_thickness;
		int _point_size;
		
		color get_draw_color()
		{
			color rs;
			SDL_GetRenderDrawColor(_rdr, &rs[0], &rs[1], &rs[2], &rs[3]);

			return rs;
		};
		void set_draw_color(const color& color)
		{
			_draw_color = color;
			SDL_SetRenderDrawColor(_rdr, color[0], color[1], color[2], color[3]);
		};

		void set_cam(bool usingcam) {_using_cam = usingcam;};
		void point(const mmath::fvec2&);
		void point(float x, float y);

		void line(const mmath::fvec2& p1, const mmath::fvec2& p2);
		void line(float x1, float y1, float x2, float y2);
		
		void circle(const mmath::circle&, bool filled);
		void circle(float cx, float cy, float radius, bool filled);
		
		void rect(const mmath::frect&, bool filled);
		void rect(float x, float y, float width, float height, bool filled);
		
		void texture(const sdl::texture&, 
				const mmath::frect& dst = {0,0,0,0},
				const mmath::irect& src = {0,0,0,0},
				double angle = 0,
				const mmath::fvec2 pivot = {0,0},
				unsigned flipflags = 0);
		
		enum class TEXT_ALIGNMENT
		{
			LEFT, CENTER, RIGHT
		};
		void text_solid(const mmath::fvec2&,
				const sdl::font&, 
				const std::string&, 
				float scale,
				int wraplength = 0,
				TEXT_ALIGNMENT alignment = TEXT_ALIGNMENT::LEFT);
		
		void text_blended(const mmath::fvec2&,
				const sdl::font&, 
				const std::string&, 
				float scale,
				int wraplength = 0,
				TEXT_ALIGNMENT alignment = TEXT_ALIGNMENT::LEFT);

		void text(const mmath::fvec2&,
				const sdl::texture& text,
				float scale,
				TEXT_ALIGNMENT alignment = TEXT_ALIGNMENT::LEFT);
		void tilemap(const tiled::tilemap&, const mmath::fvec2&, const std::vector<sdl::texture*>&);
		void tilemap(const beaver::tile::tilemap&, const mmath::fvec2&, const std::vector<sdl::texture>&);
		void tilemap_by_layer(const beaver::tile::tilemap&, const std::string&, const mmath::fvec2&, const std::vector<sdl::texture>&);
	};
};
#endif
