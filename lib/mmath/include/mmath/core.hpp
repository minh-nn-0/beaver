#ifndef NNM_MATHCORE_H
#define NNM_MATHCORE_H

#include "linear_algebra.hpp"
#include "geometry.hpp"

namespace mmath
{
	constexpr bool point_in_rect(const mmath::fvec2& point, const mmath::frect& rect)
	{
		return point.x >= rect._pos.x
			&& point.x <= rect._pos.x + rect._size.x
			&& point.y >= rect._pos.y
			&& point.y <= rect._pos.y + rect._size.y;
	};

	constexpr bool rect_intersecting(const mmath::frect& A, const mmath::frect& B)
	{
		return A._pos.x < B._pos.x + B.width() &&
			A._pos.x + A.width() > B._pos.x &&
			A._pos.y < B._pos.y + B.height() &&
			A._pos.y + A.height() > B._pos.y;
	};
};
#endif
