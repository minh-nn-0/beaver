#pragma once

#include <array>

namespace beaver::component
{
	struct color
	{
		using value_t = std::array<unsigned char, 4>;
		std::array<unsigned char, 4> _value;
	};

	struct color_keyframe
	{
		float _time; // normalized between 0 and 1;
		color _color;
	};
};
