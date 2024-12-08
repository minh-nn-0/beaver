#include <Beaver/graphics.hpp>

beaver::graphics::graphics(SDL_Window* wd, SDL_Renderer* rdr, camera2D* cam)
	: _wd(wd), _rdr(rdr), _cam(cam)
{
};


void beaver::graphics::point(const mmath::fvec2& p)
{
	mmath::fvec2 draw_point {p};
	if (_cam != nullptr) draw_point -= _cam->_view._pos;
	SDL_RenderDrawPointF(_rdr, draw_point.x, draw_point.y);
};

void beaver::graphics::point(float x, float y)
{
	point({x,y});
};

void beaver::graphics::line(const mmath::fvec2& p1, const mmath::fvec2& p2)
{
	mmath::fvec2 draw_point1 {p1};
	mmath::fvec2 draw_point2 {p2};
	if (_cam != nullptr)
	{
		draw_point1 -= _cam->_view._pos;
		draw_point2 -= _cam->_view._pos;
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
	if (_cam != nullptr) drawrect._pos -= _cam->_view._pos;
	SDL_FRect sdl_drawrect = drawrect;
	if (filled) 
		SDL_RenderFillRectF(_rdr, &sdl_drawrect);
	else 
		SDL_RenderDrawRectF(_rdr, &sdl_drawrect);
};

void beaver::graphics::rect(float x, float y, float width, float height, bool filled)
{
	rect({x,y,width,height}, filled);
};
void beaver::graphics::circle(const mmath::circle& circle, bool filled)
{
// Code stolen from https://gist.github.com/Gumichan01/332c26f6197a432db91cc4327fcabb1c, with a little modifying
 //   CHECK_RENDERER_MAGIC(renderer, -1);

	int offsetx = 0;
    int offsety = circle._radius;
    int d = circle._radius -1;

	float x = circle._center.x, y = circle._center.y;
	if (_cam != nullptr)
	{
		x -= _cam->_view._pos.x;
		y -= _cam->_view._pos.y;
	};
	if (filled)
	{
		while (offsety >= offsetx) 
		{
			line({x - offsety, y + offsetx}, {x + offsety, y + offsetx});
			line({x - offsetx, y + offsety}, {x + offsetx, y + offsety});
			line({x - offsetx, y - offsety}, {x + offsetx, y - offsety});
			line({x - offsety, y - offsetx}, {x + offsety, y - offsetx});

			if (d >= 2*offsetx) {
				d -= 2*offsetx + 1;
				offsetx +=1;
			}
			else if (d < 2 * (circle._radius - offsety)) {
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
			else if (d < 2 * (circle._radius - offsety)) {
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

	SDL_Rect* sdlsrc = sdlsrc_.x == 0 ? nullptr : &sdlsrc_; 
	SDL_FRect* sdldst = sdldst_.x == 0 ? nullptr : &sdldst_; 

	if (_cam != nullptr)
	{
		sdldst->x -= _cam->_view._pos.x;
		sdldst->y -= _cam->_view._pos.y;
	};
	SDL_FPoint p;
	p.x = pivot.x;
	p.y = pivot.y;

	SDL_RenderCopyExF(_rdr, tex, sdlsrc, sdldst, angle, &p, static_cast<SDL_RendererFlip>(flipflags));
};

void beaver::graphics::text_solid(const mmath::fvec2& pos,
				const sdl::font& font,
				const std::string& content,
				int wraplength)
{
	SDL_Color fg {_draw_color[0], _draw_color[1], _draw_color[2], _draw_color[3]};
	SDL_Surface* temp;
	if (wraplength == 0) temp = TTF_RenderUTF8_Solid(font, content.c_str(), fg);
	else temp = TTF_RenderUTF8_Solid_Wrapped(font, content.c_str(), fg, wraplength);
	
	sdl::texture text {SDL_CreateTextureFromSurface(_rdr, temp)};
	SDL_FreeSurface(temp);

	mmath::frect dst = {pos.x, pos.y, 
						static_cast<float>(text._width),
						static_cast<float>(text._height)};

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
					
					// adding cam offset here is wrong
					if (graphic._cam) 
						dst._pos -= (graphic._cam->_view._pos /* + cam._offset */ + offset) * parallax;
					
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

