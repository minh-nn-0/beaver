#pragma once

#include <beaver/camera.hpp>
#include <sol/sol.hpp>
namespace beaver::scripting
{
	void bind_camera(camera2D& cam, sol::table& tbl);
};
