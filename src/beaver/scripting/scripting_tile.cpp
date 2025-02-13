#include <beaver/scripting/scripting_tile.hpp>

void check_valid_mapname(beaver::scripting::map_container& maps, std::size_t index)
{
	if (index >= maps.size()) throw std::out_of_range(std::format("map index {} out of bound, map container size: {}", index, maps.size()));
};

std::string get_layer_type(beaver::tile::tilemap* map, const std::string& layername)
{
	if (std::holds_alternative<beaver::tile::tilelayer>(map->get_layer(layername)._data)) return "tilelayer";
	if (std::holds_alternative<beaver::tile::image_layer>(map->get_layer(layername)._data)) return "tilelayer";
	else return "group";
};
void beaver::scripting::bind_tile(sdlgame& game, map_container& maps, sol::table& tbl)
{
	tbl.set_function("new_map", [&](const std::string& map_path)
			{
				if (!std::filesystem::exists(map_path)) throw std::runtime_error(std::format("path {} not found", map_path));
				maps.emplace_back(std::filesystem::path(map_path));
				return maps.size() - 1;
			});
	tbl.set_function("get_map_size", [&](tile::tilemap* map)
			{
				return std::make_pair(map->_numx, map->_numy);
			});
	tbl.set_function("get_layer_visible", [&](tile::tilemap* map, const std::string& layer_name) -> bool
			{
				return map->get_layer(layer_name)._visible;
			});
	tbl.set_function("get_layer_tint", [&](tile::tilemap* map, const std::string& layer_name)
			{
				return map->get_layer(layer_name)._drawdata._tint;
			});
	tbl.set_function("get_layer_parallax", [&](tile::tilemap* map, const std::string& layer_name) -> std::pair<float,float>
			{
				const auto& prl = map->get_layer(layer_name)._drawdata._parallax;
				return std::make_pair(prl.x, prl.y);
			});
	tbl.set_function("get_layer_offset", [&](tile::tilemap* map, const std::string& layer_name) -> std::pair<float, float>
			{
				const auto& ofs = map->get_layer(layer_name)._drawdata._offset;
				return std::make_pair(ofs.x, ofs.y);
			});

	tbl.set_function("set_layer_tint", [&](tile::tilemap* map, const std::string& layer_name, 
				unsigned char r, unsigned char g, unsigned char b, unsigned char a)
			{
				map->get_layer(layer_name)._drawdata._tint = {r,g,b,a};
			});
	tbl.set_function("set_layer_parallax", [&](tile::tilemap* map, const std::string& layer_name, 
				float x, float y)
			{
				map->get_layer(layer_name)._drawdata._parallax = {x,y};
			});
	tbl.set_function("set_layer_offset", [&](tile::tilemap* map, const std::string& layer_name, 
				float x, float y)
			{
				map->get_layer(layer_name)._drawdata._offset = {x,y};
			});

	tbl.set_function("set_layer_visible", [&](tile::tilemap* map, const std::string& layer_name, bool visible)
			{
				map->get_layer(layer_name)._visible = visible;
			});

	// GROUPLAYER FUNCTIONS
	
	tbl.set_function("get_group_layers", [&](tile::tilemap* map, const std::string& layer_name) -> std::vector<std::string>
			{
				if (auto* gr = std::get_if<tile::group>(&map->get_layer(layer_name)._data))
					return gr->_layers;
				else throw std::runtime_error(std::format("layer {} is not a group layer", layer_name));
			});

	// TILELAYER FUNCTIONS
	tbl.set_function("get_flipflags", [&](long id) {return tiled::get_flipflags(id);});
	tbl.set_function("get_tile", [&](tile::tilemap* map, const std::string& layer_name, 
				int tilex, int tiley)
			{
				if (get_layer_type(map, layer_name) != "tilelayer")
					throw std::logic_error(std::format("layer {} is not a tilelayer", layer_name));
				auto& tl = std::get<tilelayer>(map->get_layer(layer_name)._data);

				return tl._data.at(get_tile_number(tilex, tiley, 
										map->_numx, 
										map->_numy));
			});
	tbl.set_function("set_tile", [&](tile::tilemap* map, const std::string& layer_name, 
				int tilex, int tiley, long tile_to_set)
			{
				if (get_layer_type(map, layer_name) != "tilelayer")
					throw std::logic_error(std::format("layer {} is not a tilelayer", layer_name));
				auto& tl = std::get<tilelayer>(map->get_layer(layer_name)._data);

				int tilenumber = get_tile_number(tilex, tiley, map->_numx, map->_numy);
				tl._data.at(tilenumber) = tile_to_set;
			});
};
