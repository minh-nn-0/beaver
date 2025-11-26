#ifndef M_CAMERA_H
#define M_CAMERA_H

#include <mmath/core.hpp>
namespace beaver
{
	struct camera2D
	{
		mmath::frect		_view {0,0,500,500};
		// offset when use with target()
		mmath::fvec2		_offset {0,0};
		float				_zoom {1};
		float			  	_rotation;
		float			  	_smooth_speed = 8;

		void target(const mmath::fvec2& target, float dt)
		{
			mmath::fvec2 itarget = mmath::fvec2{std::round(target.x), std::round(target.y)}; 
			// TODO _zoom factor is troublesome
			mmath::fvec2 destination = (itarget - _view._size / 2.f / _zoom + _offset) - _view._pos; 
			//TODO translate cam pos to destination
			
			//DOne that day <---- actually not
			if (_view._pos.distance(destination) > 0.5)
				_view._pos += destination * _smooth_speed * dt;
			else _view._pos = destination;
		};

		mmath::fvec2 center()
		{
			return _view._size / 2.f / _zoom + _offset;
		};
	};
};
#endif
