#pragma once
#include <beaver/ecs/entity.hpp>
#include <sol/sol.hpp>
namespace beaver::scripting
{
	template<typename... Ts>
	void bind_entity(beaver::ecs<Ts...>& ecs, sol::table& tbl)
	{
		tbl.set_function("add_entity", [&]{return ecs.add_entity();});
		tbl.set_function("entities_size", [&]{return ecs.size();});
		tbl.set_function("get_active_entities", [&] -> std::vector<std::size_t> {return ecs.active_entities();});
		tbl.set_function("set_active",[&](std::size_t eid, bool active) {ecs.set_active(eid, active);});
		tbl.set_function("is_active", [&](std::size_t eid) {return ecs.is_active(eid);});
		tbl.set_function("reset_entity", [&](std::size_t eid) {ecs.reset_entity(eid);});
		tbl.set_function("add_tag", [&](std::size_t eid, const std::string& tag) {ecs.add_tag(eid, tag);});
		tbl.set_function("remove_tag", [&](std::size_t eid, const std::string& tag) {ecs.remove_tag(eid, tag);});
		tbl.set_function("has_tag", [&](std::size_t eid, const std::string& tag) {return ecs.has_tag(eid, tag);});
		tbl.set_function("get_entities_with_tags", [&](const sol::table& tags) -> std::vector<std::size_t>
				{
					std::vector<std::string> tags_list;
					for (const auto& pair: tags) tags_list.push_back(pair.second.as<std::string>());
					return ecs.get_with_tags(tags_list);
				});
	};
	
};
