#ifndef M_MAP_H
#define M_MAP_H

#include <variant>
#include <bitset>
#include <format>
#include <array>

#include <headeronly/json.hpp>
#include <mmath/core.hpp>
#include <utilities.hpp>

namespace tiled
{
	template<typename T>
	concept tiling = requires (T x) {x._tilesize; x._numx; x._numy;};

	template<typename T>
	concept tilebase = requires (T x) {x._tileid;};

	// ===================== FLIP FLAG HANDLING ======================
	constexpr unsigned TILED_FLIPFLAG_H = 0x80000000;
	constexpr unsigned TILED_FLIPFLAG_V = 0x40000000;
	constexpr unsigned TILED_FLIPFLAG_D = 0x20000000; // no equivalent in sdl

	constexpr unsigned TILED_FLIPFLAG_MASK = TILED_FLIPFLAG_H | TILED_FLIPFLAG_V | TILED_FLIPFLAG_D;

	constexpr std::pair<unsigned, int> get_flipflags (const long& id)
	{
		unsigned flags {0};
		if (id & TILED_FLIPFLAG_H) flags |= 0x00000001;
		if (id & TILED_FLIPFLAG_V) flags |= 0x00000002;

		return {flags, id &~ TILED_FLIPFLAG_MASK};
	};
	


	// =========================== DRAWING ===========================

	struct drawdata
	{
		drawdata() = default;
		
		drawdata(float opacity, mmath::fvec2 parallax, mmath::fvec2 offset, std::array<float, 4> tint)
			: _opacity(opacity), _parallax(parallax), _offset(offset), _tint(tint) {};

		drawdata(const nlohmann::json&);
		
		float					_opacity = 1.0f;
		mmath::fvec2			_parallax = {1.0f, 1.0f};
		mmath::fvec2			_offset = {0, 0}; 
		std::array<float, 4>	_tint = {1.0, 1.0, 1.0, 1.0};

		drawdata& operator+= (const drawdata& d)
		{
			_opacity = _opacity * d._opacity;
			_parallax = _parallax * d._parallax;
			_offset	= _offset + d._offset;
			_tint = utils::blend_rgba_multiply(_tint, d._tint);

			return *this;
		};

		drawdata operator+ (const drawdata& d) const
		{
			return {_opacity * d._opacity,
					_parallax * d._parallax,
					_offset + d._offset,
					 utils::blend_rgba_multiply(_tint, d._tint)};

		};
	};

	// =========================== PRIMITIVES ========================
	using tile = long;


	using rect = mmath::frect;
	//struct polygon;
	using point = mmath::fvec2;
	//struct ellipse;

	struct object
	{
		std::string _name;
		std::variant<rect, point, tile> _object;
		nlohmann::json _properties;
	};

	// ============================ LAYERS ============================
	struct __layer
	{
		std::string				_name;
		drawdata				_drawdata;
		bool 					_visible {true};
	};

	struct tilelayer : __layer
	{
		std::vector<tile>	_data;
		int _tilesize, _numx, _numy;
	};

	struct objectlayer : __layer
	{
		std::vector<object> _objects;
	};

	struct grouplayer : __layer
	{
		using layer = std::variant<tilelayer, objectlayer, grouplayer>;
		std::vector<layer> _layers;
		
		bool have_layer(const std::string& name) const
		{
			return std::ranges::find_if(_layers,
						[&](auto&& layer) 
						{ return std::visit(
									[&](auto&& layer) {return layer._name == name;}, 
									layer); }) != _layers.end();
		};

		template<typename T> requires std::is_base_of_v<__layer,T>
		T& get_layer_by_name(const std::string& name) 
		{
			if (auto rs = std::ranges::find_if(
						_layers,
						[&](auto&& layer) 
						{ return std::visit(
									[&](auto&& layer) {return layer._name == name;}, 
									layer);
						});
				rs != _layers.end() && std::holds_alternative<T>(*rs))
				return *rs;
			else throw std::runtime_error(std::format("layer of name {} not found", name));
		};
	};


	using layer = std::variant<tilelayer, objectlayer, grouplayer>;

	bool layer_is_visible(const auto& layer)
	{
		return std::visit([](auto&& ly){return ly._visible;}, layer);
	};
	// =================== TILESET ===========================
	
	struct tileset
	{
		std::filesystem::path _img;
		int _tilesize, _numx, _numy;
	};
	
	// =================== TILEMAP ==========================
	struct tilemap
	{
		tilemap() = default;
		tilemap(const std::filesystem::path&);
		
		std::map<int, tileset> _tilesets;
		grouplayer _layerdata;
		std::array<int, 4> _bgcolor {0,0,0,255};
		nlohmann::json _properties;
		std::filesystem::path _path;
		int	_tilesize, _numx, _numy;
		
		const tileset& tileset_at(int id) const
		{
			return std::ranges::find_if(_tilesets | std::views::reverse,
						[=](auto&& tid){return tid.first <= id;})->second; 
		};
	};





	template<Numeric T>
	mmath::ivec2 id_at(const mmath::vec2<T>& v, int tilesize)
	{
		int idx = v.x / tilesize;
		int idy = v.y / tilesize;

		return {idx, idy};
	};
	// Given an index and anything that have _numx, _numy, _tilesize, 
	// return rectangle at given index
	inline mmath::irect rect_at(int index, const tiling auto& src)
	{
		auto div = std::div(index, src._numx);
		if (div.quot > src._numy) throw std::out_of_range {std::format("invalid id {}, exceed height of tile resource {}", index, src._numy)};
		
		return mmath::irect
		{
			{div.rem * src._tilesize, div.quot * src._tilesize},
			{src._tilesize, src._tilesize}
		};
	};
	
	// Given an index, width, height and tilesize
	// return rectangle at given index
	inline mmath::irect rect_at(int index, int tilesize, int numx, int numy)
	{
		auto div = std::div(index, numx);
		if (div.quot > numy) throw std::out_of_range {std::format("invalid id {}, exceed height of tile resource {}", index, numy)};
		
		return mmath::irect
		{
			{div.rem * tilesize, div.quot * tilesize},
			{tilesize, tilesize}
		};
	};

	inline mmath::irect rect_at(const mmath::ivec2& index, const tiling auto& src)
	{
		return mmath::irect
		{
			{index.x * src._tilesize, index.y * src._tilesize},
			{src._tilesize, src._tilesize}
		};
	};
};
#endif
