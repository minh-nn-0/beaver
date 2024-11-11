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
		float			  	_smooth_speed = 0.06;

		void target(const mmath::fvec2& target, float dt)
		{
			// TODO _zoom factor is troublesome
			mmath::fvec2 destination = (target - _view._size / 2 / _zoom - _view._pos - _offset) ;
			//TODO translate cam pos to destination
			
			//DOne that day <---- actually not
			_view._pos += destination * _smooth_speed * dt;
		};
	};
};
#endif
