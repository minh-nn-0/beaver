#pragma once

#include <beaver/sdlgame.hpp>
#include <sol/sol.hpp>

namespace beaver::scripting
{
	void init_lua(sol::state& lua);
	void bind_core(beaver::sdlgame&, sol::state& lua);
};
