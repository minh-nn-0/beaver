#include <beaver/scripting/scripting_tile.hpp>

void check_valid_mapname(beaver::scripting::map_container& maps, const std::string& mapname)
{
	if (!maps.contains(mapname)) throw std::invalid_argument(std::format("mapname {} not found in container", mapname));
};

std::string get_layer_type(beaver::scripting::map_container& maps, const std::string& mapname, const std::string& layername)
{
	check_valid_mapname(maps, mapname);
	if (std::holds_alternative<beaver::tile::tilelayer>(maps.at(mapname).get_layer(layername)._data)) return "tilelayer";
	else return "group";
};
void beaver::scripting::bind_tile(sdlgame& game, map_container& maps, sol::table& tbl)
{
	tbl.set_function("new_map", [&](const std::string& map_name, const std::string& map_path)
			{
				maps.emplace(map_name, tilemap{map_path});
			});
	tbl.set_function("get_map_size", [&](const std::string& map_name) -> std::pair<int, int>
			{
				check_valid_mapname(maps, map_name);
				return std::make_pair(maps.at(map_name)._numx, maps.at(map_name)._numy);
			});
	tbl.set_function("get_layer_visible", [&](const std::string& map_name, const std::string& layer_name) -> bool
			{
				check_valid_mapname(maps, map_name);
				return maps.at(map_name).get_layer(layer_name)._visible;
			});
	tbl.set_function("get_layer_tint", [&](const std::string& map_name, const std::string& layer_name)
			{
				check_valid_mapname(maps, map_name);
				return maps.at(map_name).get_layer(layer_name)._drawdata._tint;
			});
	tbl.set_function("get_layer_parallax", [&](const std::string& map_name, const std::string& layer_name) -> std::pair<float,float>
			{
				check_valid_mapname(maps, map_name);
				const auto& prl = maps.at(map_name).get_layer(layer_name)._drawdata._parallax;
				return std::make_pair(prl.x, prl.y);
			});
	tbl.set_function("get_layer_offset", [&](const std::string& map_name, const std::string& layer_name) -> std::pair<float, float>
			{
				check_valid_mapname(maps, map_name);
				const auto& ofs = maps.at(map_name).get_layer(layer_name)._drawdata._offset;
				return std::make_pair(ofs.x, ofs.y);
			});

	tbl.set_function("set_layer_tint", [&](const std::string& map_name, const std::string& layer_name, 
				unsigned char r, unsigned char g, unsigned char b, unsigned char a)
			{
				check_valid_mapname(maps, map_name);
				maps.at(map_name).get_layer(layer_name)._drawdata._tint = {r,g,b,a};
			});
	tbl.set_function("set_layer_parallax", [&](const std::string& map_name, const std::string& layer_name, 
				float x, float y)
			{
				check_valid_mapname(maps, map_name);
				maps.at(map_name).get_layer(layer_name)._drawdata._parallax = {x,y};
			});
	tbl.set_function("set_layer_offset", [&](const std::string& map_name, const std::string& layer_name, 
				float x, float y)
			{
				check_valid_mapname(maps, map_name);
				maps.at(map_name).get_layer(layer_name)._drawdata._offset = {x,y};
			});

	tbl.set_function("set_layer_visible", [&](const std::string& map_name, const std::string& layer_name, bool visible)
			{
				check_valid_mapname(maps, map_name);
				maps.at(map_name).get_layer(layer_name)._visible = visible;
			});

	// GROUPLAYER FUNCTIONS
	
	tbl.set_function("get_group_layers", [&](const std::string& map_name, const std::string& layer_name) -> std::vector<std::string>
			{
				if (auto* gr = std::get_if<tile::group>(&maps.at(map_name).get_layer(layer_name)._data))
					return gr->_layers;
				else throw std::runtime_error(std::format("map {} layer {} is not a group layer", map_name, layer_name));
			});

	// TILELAYER FUNCTIONS
	tbl.set_function("get_flipflags", [&](long id) {return tiled::get_flipflags(id);});
	tbl.set_function("get_tile", [&](const std::string& map_name, const std::string& layer_name, 
				int tilex, int tiley)
			{
				check_valid_mapname(maps, map_name);
				if (get_layer_type(maps, map_name, layer_name) != "tilelayer")
					throw std::logic_error(std::format("layer {} of map {} is not a tilelayer", layer_name, map_name));
				auto& tl = std::get<tilelayer>(maps.at(map_name).get_layer(layer_name)._data);

				return tl._data.at(get_tile_number(tilex, tiley, 
										maps.at(map_name)._numx, 
										maps.at(map_name)._numy));
			});
	tbl.set_function("set_tile", [&](const std::string& map_name, const std::string& layer_name, 
				int tilex, int tiley, long tile_to_set)
			{
				check_valid_mapname(maps, map_name);
				if (get_layer_type(maps, map_name, layer_name) != "tilelayer")
					throw std::logic_error(std::format("layer {} of map {} is not a tilelayer", layer_name, map_name));
				auto& tl = std::get<tilelayer>(maps.at(map_name).get_layer(layer_name)._data);

				int tilenumber = get_tile_number(tilex, tiley, maps.at(map_name)._numx, maps.at(map_name)._numy);
				tl._data.at(tilenumber) = tile_to_set;
			});
};
