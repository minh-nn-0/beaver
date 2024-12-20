#ifndef M_GEO_H
#define M_GEO_H

#include <iostream>
#include "linear_algebra.hpp"
#include <SDL.h>
namespace mmath
{
	template<typename UnitT>
	struct rectangle
	{
		rectangle() = default;
		rectangle(UnitT x, UnitT y, UnitT w, UnitT h)
		{
			_pos = {x,y};
			_size = {w,h};
		};
		rectangle(vec2<UnitT> x, vec2<UnitT> y)
		{
			_pos = x;
			_size = y;
		};
		vec2<UnitT> _pos;
		vec2<UnitT> _size;
		
		template<Numeric N>
		rectangle<N> operator * (const N& scale) const
		{
			return {_pos * scale, _size * scale};
		};

		auto operator <=> (const rectangle&) const = default;

		template<Numeric T>
		operator rectangle<T>()
		{
			return rectangle<T>
			{
				static_cast<vec2<T>>(_pos),
				static_cast<vec2<T>>(_size),
			};
		};

		UnitT 		width() 	const {return _size.x;};
		UnitT 		height()	const {return _size.y;};
		fvec2 		center() 	const {return _pos + _size/2;};
		vec2<UnitT> side_h() 	const {return {_pos.x, _pos.x + width()};};
		vec2<UnitT> side_v() 	const {return {_pos.y, _pos.y + height()};};


		operator SDL_Rect() const
		{
			return SDL_Rect{.x = static_cast<int>(_pos.x),
							.y = static_cast<int>(_pos.y),
							.w = static_cast<int>(_size.x),
							.h = static_cast<int>(_size.y)};
		};
		operator SDL_FRect() const
		{
			return SDL_FRect{.x = static_cast<float>(_pos.x),
							 .y = static_cast<float>(_pos.y),
							 .w	= static_cast<float>(_size.x),
							 .h = static_cast<float>(_size.y)};
		};
	};
	

	using irect = rectangle<int>;
	using frect = rectangle<float>;

	struct circle
	{
		fvec2 _center;
		float _radius;
	};
	
}
#endif
