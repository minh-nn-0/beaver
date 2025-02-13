#pragma once

#include <beaver/tile.hpp>
#include <sol/sol.hpp>
#include <beaver/sdlgame.hpp>
namespace beaver::scripting
{
	using namespace tile;
	using map_container = std::vector<tilemap>;
	void bind_tile(sdlgame& game, map_container& maps, sol::table& tbl);
};
