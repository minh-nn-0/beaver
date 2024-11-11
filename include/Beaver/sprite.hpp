#ifndef BEAVER_SPRITE_H
#define BEAVER_SPRITE_H

#include <vector>
#include <mmath/core.hpp>
#include <Beaver/tileanimation.hpp>
namespace beaver
{
	struct sprite
	{
		int							_tileid;
		mmath::frect				_rect;
		//relative to _rect
		std::vector<mmath::frect> 	_collisions;
		beaver::tile_animation 		_tileanimation;
	};
};

#endif
