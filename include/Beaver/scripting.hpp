#ifndef BEAVER_SCRIPTING_H
#define BEAVER_SCRIPTING_H

#include <sol/sol.hpp>
#include <mmath/core.hpp>

// ================= Mainly dealing with table in Lua ================ 

namespace beaver
{
	inline mmath::irect table_to_irect(const sol::table& tbl)
	{
		return {
			tbl.get_or("x", 0), tbl.get_or("y",0),
			tbl.get_or("w", 0), tbl.get_or("h",0)
		};
	};
	inline mmath::frect table_to_frect(const sol::table& tbl)
	{
		return {
			tbl.get_or("x", 0.f), tbl.get_or("y",0.f),
			tbl.get_or("w", 0.f), tbl.get_or("h",0.f)
		};
	};
	inline sol::table rect_to_table(const mmath::frect& rect)
	{
		sol::table rs;
		rs["x"] = rect._pos.x;
		rs["y"] = rect._pos.x;
		rs["w"] = rect._pos.x;
		rs["h"] = rect._pos.x;
		return rs;
	};

	void load_scripting_system(sol::state& lua);
};

#endif
