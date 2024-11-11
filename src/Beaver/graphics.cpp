#include <Beaver/graphics.hpp>

void beaver::graphics::draw::point(renderer& rd, const mmath::fvec2& p)
{
	SDL_RenderDrawPointF(rd, p.x, p.y);
};
void beaver::graphics::draw::line(renderer& rd, const mmath::fvec2& p1, const mmath::fvec2& p2)
{
	SDL_RenderDrawLineF(rd, p1.x, p1.y, p2.x, p2.y);
};
void beaver::graphics::draw::rect(renderer& rd, const mmath::frect& rect, drawmode dm)
{
	switch (dm)
	{
		case drawmode::fill:
		{
			SDL_FRect drawrect = rect;
			SDL_RenderFillRectF(rd, &drawrect);
			break;
		}
		case drawmode::line:
		{
			SDL_FRect drawrect = rect;
			SDL_RenderDrawRectF(rd, &drawrect);
			break;
		}
	};
};
void beaver::graphics::draw::circle(renderer&)
{
};
void beaver::graphics::draw::sprite(renderer&)
{
};
void beaver::graphics::draw::text(renderer& rd, const sdl::font& font, const std::string&)
{
};
void beaver::graphics::draw::tilemap(renderer& rd, const tiled::tilemap& tm)
{
};
