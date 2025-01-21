#include <beaver/graphics.hpp>

beaver::graphics::graphics(SDL_Window* wd, SDL_Renderer* rdr, camera2D* cam)
	: _wd(wd), _rdr(rdr), _cam(cam)
{
};

void beaver::graphics::point(const mmath::fvec2& p)
{
	mmath::fvec2 draw_point {p};
	if (_cam != nullptr && _using_cam)
	{
		rect(draw_point.x, draw_point.y, 1, 1, true);
	}
	else SDL_RenderDrawPointF(_rdr, draw_point.x, draw_point.y);
};

void beaver::graphics::point(float x, float y)
{
	point({x,y});
};

void beaver::graphics::line(const mmath::fvec2& p1, const mmath::fvec2& p2)
{
	mmath::fvec2 draw_point1 {p1};
	mmath::fvec2 draw_point2 {p2};
	if (_cam != nullptr && _using_cam)
	{
		draw_point1 = position_with_cam(draw_point1, *_cam);
		draw_point2 = position_with_cam(draw_point2, *_cam);
	}


	SDL_RenderDrawLineF(_rdr, draw_point1.x, draw_point1.y, draw_point2.x, draw_point2.y);
};

void beaver::graphics::line(float x1, float y1, float x2, float y2)
{
	line({x1,y1},{x2,y2});
};

void beaver::graphics::rect(const mmath::frect& rect, bool filled)
{
	mmath::frect drawrect = rect;
	if (_cam != nullptr && _using_cam) 
	{
		drawrect._pos = position_with_cam(drawrect._pos, *_cam);
		drawrect._size = drawrect._size * _cam->_zoom;
	};
	SDL_FRect sdl_drawrect = drawrect;

	SDL_FRect* p_sdl_drawrect = sdl_drawrect.w <= 0 || sdl_drawrect.h <= 0 ? nullptr : &sdl_drawrect; 
	if (filled) 
		SDL_RenderFillRectF(_rdr, p_sdl_drawrect);
	else 
		SDL_RenderDrawRectF(_rdr, p_sdl_drawrect);
};

void beaver::graphics::rect(float x, float y, float width, float height, bool filled)
{
	rect({x,y,width,height}, filled);
};
void beaver::graphics::circle(const mmath::circle& circle, bool filled)
{
// Code stolen from https://gist.github.com/Gumichan01/332c26f6197a432db91cc4327fcabb1c, with a little modifying
 //   CHECK_RENDERER_MAGIC(renderer, -1);
	
	if (circle._radius <= 1) point(circle._center.x, circle._center.y);
	else
	{
		mmath::fvec2 center = circle._center;
		float radius = circle._radius;

		if (_cam != nullptr && _using_cam)
		{
			//center = position_with_cam(center, *_cam);
			//radius *= _cam->_zoom;
		};
		int offsetx = 0;
		int offsety = radius;
		int d = radius - 1;

		float x = center.x, y = center.y;
		if (filled)
		{
			while (offsety >= offsetx) 
			{
				for (int dx = x - offsetx; dx <= x + offsetx; dx++) {
					point(dx, y + offsety); // Top half
					point(dx, y - offsety); // Bottom half
				}
				for (int dx = x - offsety; dx <= x + offsety; dx++) {
					point(dx, y + offsetx); // Right side
					point(dx, y - offsetx); // Left side
				}
				//for (int dx = -y; dx <= y; dx++) {
				//    // Vertical line at x = offsety
				//    rect(center.x + dx, center.y + x, 1, 1, true);
				//    rect(center.x + dx, center.y - x, 1, 1, true);
				//}
				//line({x - offsety, y + offsetx}, {x + offsety, y + offsetx});
				//line({x - offsetx, y + offsety}, {x + offsetx, y + offsety});
				//line({x - offsetx, y - offsety}, {x + offsetx, y - offsety});
				//line({x - offsety, y - offsetx}, {x + offsety, y - offsetx});

				if (d >= 2*offsetx) {
					d -= 2*offsetx + 1;
					offsetx +=1;
				}
				else if (d < 2 * (radius - offsety)) {
					d += 2 * offsety - 1;
					offsety -= 1;
				}
				else {
					d += 2 * (offsety - offsetx - 1);
					offsety -= 1;
					offsetx += 1;
				}
			}
		}
		else
		{
			while (offsety >= offsetx)
			{
				point({x + offsetx, y + offsety});
				point({x + offsety, y + offsetx});
				point({x - offsetx, y + offsety});
				point({x - offsety, y + offsetx});
				point({x + offsetx, y - offsety});
				point({x + offsety, y - offsetx});
				point({x - offsetx, y - offsety});
				point({x - offsety, y - offsetx});

				if (d >= 2*offsetx) {
					d -= 2*offsetx + 1;
					offsetx +=1;
				}
				else if (d < 2 * (radius - offsety)) {
					d += 2 * offsety - 1;
					offsety -= 1;
				}
				else {
					d += 2 * (offsety - offsetx - 1);
					offsety -= 1;
					offsetx += 1;
				}
			}
		}
	}
}

void beaver::graphics::circle(float cx, float cy, float radius, bool filled)
{
	circle({cx,cy,radius},filled);
};

void beaver::graphics::texture(const sdl::texture& tex, 
				const mmath::frect& dst,
				const mmath::irect& src,
				double angle,
				const mmath::fvec2 pivot,
				unsigned flipflags)
{
	SDL_Rect sdlsrc_ = src;
	SDL_FRect sdldst_ = dst;


	if (_cam != nullptr && _using_cam)
	{
		mmath::fvec2 pos = position_with_cam(dst._pos, *_cam);
		sdldst_.x = pos.x;
		sdldst_.y = pos.y;

		sdldst_.w = sdldst_.w * _cam->_zoom;
		sdldst_.h = sdldst_.h * _cam->_zoom;
	};
	SDL_Rect* sdlsrc = sdlsrc_.w == 0 ? nullptr : &sdlsrc_; 
	SDL_FRect* sdldst = sdldst_.w == 0 ? nullptr : &sdldst_; 
	SDL_FPoint p;
	p.x = pivot.x;
	p.y = pivot.y;

	SDL_RenderCopyExF(_rdr, tex, sdlsrc, sdldst, angle, &p, static_cast<SDL_RendererFlip>(flipflags));
};

void beaver::graphics::text_solid(const mmath::fvec2& pos,
				const sdl::font& font,
				const std::string& content,
				float scale,
				int wraplength,
				TEXT_ALIGNMENT alignment)
{
	text(pos, make_text_solid(_rdr, font, content, _draw_color, wraplength), scale, alignment);
};
void beaver::graphics::text_blended(const mmath::fvec2& pos,
				const sdl::font& font,
				const std::string& content,
				float scale,
				int wraplength,
				TEXT_ALIGNMENT alignment)
{
	text(pos, make_text_blended(_rdr, font, content, _draw_color, wraplength), scale, alignment);
};

void beaver::graphics::text(const mmath::fvec2& pos,
		const sdl::texture& text,
		float scale,
		TEXT_ALIGNMENT alignment)
{
	mmath::frect dst = {pos.x, pos.y, 
						floor(text._width * scale),
						floor(text._height * scale)};

	switch (alignment)
	{
		case (TEXT_ALIGNMENT::LEFT): break;
		case (TEXT_ALIGNMENT::CENTER): dst._pos.x -= dst._size.x / 2 ; break;
		case (TEXT_ALIGNMENT::RIGHT): dst._pos.x -= dst._size.x; break;
	};
	texture(text, dst);
};

void draw_layers(const tiled::layer& layer,
		tiled::drawdata	parent_drawdata,
		beaver::graphics& graphic, 
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
					
					dst._pos = (dst._pos + offset) * parallax;
					// adding cam offset here is wrong
					if (graphic._cam) 
					{
						dst._pos = position_with_cam(dst._pos, *graphic._cam) /* + cam._offset */ + offset * parallax;
						dst._size = dst._size * graphic._cam->_zoom;
					};
					//dst._pos.x -= cam._view._pos.x * parallax.x - offset.x;
					//dst._pos.y -= cam._view._pos.y * parallax.y - offset.y;
					
					graphic.texture(*ts_tex, dst, src, flipflag);
					
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
				draw_layers(grouplayer, parent_drawdata, graphic, tm, textures);
			};
	};
};
void beaver::graphics::tilemap(const tiled::tilemap& tm,
		const mmath::fvec2& pos,
		const std::vector<sdl::texture*>& textures)
{
	for (const auto& layer: tm._layerdata._layers)
		draw_layers(layer, {}, *this, tm, textures);
};

using namespace beaver::tile;
void draw_tilelayer(const tilelayer& tl,
		const mmath::fvec2& pos,
		drawdata ddata,
		beaver::graphics& graphic, 
		const tilemap& tm,
		const std::vector<sdl::texture*>& textures)
{
	auto [parallax, offset, tint] = ddata;
	for (int i = 0; i != tl._data.size(); i++)
	{
		if (long tile = tl._data.at(i); tile >= 0)
		{
			auto [flipflag, tileid] = tiled::get_flipflags(tile);

			const tileset& ts = tm.tileset_at(tileid); 
			if (ts._textureid == -1)
			{
				std::println("image {} not found", ts._filename);
				return;
			};
			
			sdl::texture* ts_tex = textures.at(ts._textureid);
			SDL_SetTextureAlphaMod(*ts_tex, tint[3]);
			SDL_SetTextureColorMod(*ts_tex, tint[0], tint[1], tint[2]);
			
			mmath::irect src(tiled::rect_at(tileid, ts));
			mmath::frect dst(tiled::rect_at(i, tm));
			
			dst._pos = dst._pos + pos;
			// adding cam offset here is wrong
			//if (graphic._cam != nullptr && graphic._using_cam) 
			//{
			//	dst._pos = (position_with_cam(dst._pos, *graphic._cam) /* + cam._offset */ + offset) * parallax;
			//	dst._size = dst._size * graphic._cam->_zoom;
			//};
			//dst._pos.x -= cam._view._pos.x * parallax.x - offset.x;
			//dst._pos.y -= cam._view._pos.y * parallax.y - offset.y;
			
			graphic.texture(*ts_tex, dst, src, 0, {0,0}, flipflag);
			
			SDL_SetTextureAlphaMod(*ts_tex, 255);
			SDL_SetTextureColorMod(*ts_tex, 255, 255, 255);
		};
	};
};

void draw_imagelayer(const image_layer& il,
		const mmath::fvec2& pos,
		drawdata ddata,
		beaver::graphics& graphic, 
		const std::vector<sdl::texture*>& textures)
{
	if (il._textureid == -1)
	{
		std::println("image {} not found", il._image_name);
		return;
	}
	sdl::texture* image = textures.at(il._textureid);
	auto [parallax, offset, tint] = ddata;
	SDL_SetTextureAlphaMod(*image, tint[3]);
	SDL_SetTextureColorMod(*image, tint[0], tint[1], tint[2]);
	mmath::frect dst = {pos, {static_cast<float>(image->_width), static_cast<float>(image->_height)}};

	graphic.texture(*image, dst);
	SDL_SetTextureAlphaMod(*image, 255);
	SDL_SetTextureColorMod(*image, 255, 255, 255);

};

std::vector<std::string> extract_groups (const std::string& string)
{
	std::vector<std::string> rs;
	for (std::size_t i = 0; i!= string.size(); i++)
	{
		if (string[i] == '.') 
			rs.emplace_back(string.substr(0,i));
	};

	return rs;
};


void beaver::graphics::tilemap(const beaver::tile::tilemap& tm,
		const mmath::fvec2& pos,
		const std::vector<sdl::texture*>& textures)
{
	for (std::size_t i = 0; i != tm._layers.second.size(); i++)
	{ 
		const layer_t& layer = tm._layers.second[i];
		if (layer._visible)
		{
			drawdata parent_drawdata;
			bool parent_visible {true};
			for (const auto& parent: extract_groups(tm.get_layer_name(i)))
			{
				const layer_t& parent_layer = tm.get_layer(parent);
				assert(std::holds_alternative<group>(parent_layer._data));
				parent_drawdata = parent_drawdata + parent_layer._drawdata;
				parent_visible &= parent_layer._visible;
			};
			if (parent_visible)
			{
				if (auto tl = std::get_if<tilelayer>(&layer._data))
					draw_tilelayer(*tl, pos, layer._drawdata + parent_drawdata, *this, tm, textures);
				else if (auto il = std::get_if<image_layer>(&layer._data))
					draw_imagelayer(*il, il->_position + pos, layer._drawdata + parent_drawdata, *this, textures);
			}
		};
	};
};

void beaver::graphics::tilemap_by_layer(const beaver::tile::tilemap& tm,
		const std::string& layer_name,
		const mmath::fvec2& pos,
		const std::vector<sdl::texture*>& textures)
{
	if (!tm._layers.first.contains(layer_name))
	{
		std::println("layer {} not found", layer_name);
		return;
	}
	const auto& layer = tm.get_layer(layer_name);
	if (layer._visible)
	{
		drawdata parent_drawdata;
		bool parent_visible {true};
		for (const auto& parent: extract_groups(layer_name))
		{
			const layer_t& parent_layer = tm.get_layer(parent);
			assert(std::holds_alternative<group>(parent_layer._data));
			parent_drawdata = parent_drawdata + parent_layer._drawdata;
			parent_visible &= parent_layer._visible;
		};
		if (parent_visible)
		{
			if (auto tl = std::get_if<tilelayer>(&layer._data))
				draw_tilelayer(*tl, pos, layer._drawdata + parent_drawdata, *this, tm, textures);
			else if (auto il = std::get_if<image_layer>(&layer._data))
				draw_imagelayer(*il, il->_position + pos, layer._drawdata + parent_drawdata, *this, textures);
			else if (auto gl = std::get_if<group>(&layer._data))
				for (auto& layer: gl->_layers)
					tilemap_by_layer(tm, layer, pos, textures);
		}
	}
};

