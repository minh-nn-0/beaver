#pragma once

#include <mmath/core.hpp>
#include <sdlwrapper/sdlwrapper.hpp>
namespace beaver::component
{
	struct image_render
	{
		std::size_t _textureid;
		mmath::irect _source {0,0,0,0};
	};
};
