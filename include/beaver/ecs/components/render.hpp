#pragma once

#include <mmath/core.hpp>
namespace beaver::component
{
	struct image_render
	{
		std::string _image_name;
		mmath::irect _source {0,0,0,0};
	};
};
