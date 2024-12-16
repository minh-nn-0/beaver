#pragma once

#include <mmath/core.hpp>
namespace beaver::component
{
	struct oscillation 
	{
		mmath::fvec2 _center;
		float _timer;
		float _amplitude;  // Maximum displacement
		float _frequency;  // Cycles per second
		mmath::fvec2 _axis;     // Direction of oscillation (e.g., (0, 1) for vertical)
		enum Waveform 
		{
			SINUSOIDAL,
			TRIANGULAR,
			SQUARE
		} _waveform = SINUSOIDAL;
	};
}
