#ifndef BEAVER_TILE_H
#define BEAVER_TILE_H

#include <beaver/ecs/entity.hpp>
#include <beaver/resource.hpp>
#include <mmath/core.hpp>
#include <utilities.hpp>
#include <headeronly/json.hpp>
#include <headeronly/sol/sol.hpp>
#include <tiledwrapper/tiledwrapper.hpp>
namespace beaver::tile
{
	constexpr int get_tile_number(int tilex, int tiley, int mapwidth, int mapheight)
	{
		if (tilex < 0 || tilex >= mapwidth || tiley < 0 || tiley >= mapheight)
			throw std::out_of_range("Tile coordinates out of bounds");
		return tiley * mapwidth + tilex;
	};
	constexpr std::pair<int, int> get_tile_coords(int tilenumber, int map_width)
	{
		int tiley = tilenumber / map_width;
		int tilex = tilenumber % map_width;
		return {tilex, tiley};
	}
	struct drawdata
	{
		mmath::fvec2			_parallax = {1.0f, 1.0f};
		mmath::fvec2			_offset = {0, 0}; 
		std::array<unsigned char, 4>	_tint = {255, 255, 255, 255};

		drawdata& operator+= (const drawdata& d)
		{
			_parallax = _parallax * d._parallax;
			_offset	= _offset + d._offset;
			_tint = utils::blend_rgba_multiply(_tint, d._tint);

			return *this;
		};

		drawdata operator+ (const drawdata& d) const
		{
			return { _parallax * d._parallax,
					_offset + d._offset,
					 utils::blend_rgba_multiply(_tint, d._tint)};
		};
	};
	struct tilelayer
	{
		std::vector<long> _data;
	};

	// Placeholder for grouplayer
	struct group {};

	struct tileset
	{
		std::string _filename;
		int _tilesize, _numx, _numy;
	};

	struct layer_t
	{
		// monostate represent a group (House.Bathroom)
		std::variant<std::monostate, tilelayer> _data;
		drawdata _drawdata;
		bool _visible;
	};
	// Container of layers
	struct tilemap
	{
		using layer_manager = std::pair<
						std::unordered_map<std::string, std::size_t>,
						std::vector<layer_t>>;
		tilemap() = default;
		tilemap(const sol::table&);
		tilemap(const std::filesystem::path& tmj);
		std::map<int, tileset> _tilesets;
		layer_manager _layers;
		utils::color _bgcolor;
		int _tilesize, _numx, _numy;

		const layer_t& get_layer(const std::string& lname) const {return _layers.second.at(_layers.first.at(lname));};
		layer_t& get_layer(const std::string& lname) {return _layers.second.at(_layers.first.at(lname));};
		std::string get_layer_name(std::size_t lid) const
		{
			if (lid >= _layers.second.size()) throw std::out_of_range("layerid not found");
			return std::ranges::find_if(_layers.first, [&](auto&& l){return l.second == lid;})->first;
		};
		const tileset& tileset_at(int id) const
		{
			return std::ranges::find_if(_tilesets | std::views::reverse , [&](auto&& ts){return ts.first <= id;})->second;
		};
	};
};

/*
 * rfr.map.set_layer_color(layerid,...)
 * rfr.map.set_layer_visible(layerid,...)
 * rfr.map.set_layer_offset(layerid,...)
 *
 */
#endif
