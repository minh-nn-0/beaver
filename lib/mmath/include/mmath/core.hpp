#ifndef NNM_MATHCORE_H
#define NNM_MATHCORE_H

#include "linear_algebra.hpp"
#include "geometry.hpp"

namespace mmath
{
	constexpr bool is_intersecting(const mmath::fvec2& point, const mmath::frect& rect)
	{
		return point.x >= rect._pos.x
			&& point.x <= rect._pos.x + rect._size.x
			&& point.y >= rect._pos.y
			&& point.y <= rect._pos.y + rect._size.y;
	};
};
#endif
