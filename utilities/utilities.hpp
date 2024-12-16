#ifndef M_UTILITIES_H
#define M_UTILITIES_H
#include <sstream>
#include <array>
#include <algorithm>
#include <bitset>
#include <vector>
#include <filesystem>
#include <fstream>

#include <mmath/core.hpp>
namespace utils
{
	using color = std::array<unsigned char, 4>;
	inline std::array<unsigned char,4> hex_to_rgba(const std::string& hex) {
		std::stringstream ss;
		std::string hex_str = hex.substr(hex[0] == '#' ? 1 : 0);
		if (hex_str.length() != 6 && hex_str.length() != 8) 
			throw std::invalid_argument("bad argument, incorrect format for hex");
		
		std::array<unsigned char,4> rgba {0,0,0,255};
		for (int i = 0; i != hex_str.length()/2; i++)
		{
			ss.clear();
			ss << hex_str.substr(i*2,2);
			ss >> std::hex >> rgba[i];
		};
		return rgba;
	}

	inline std::array<float, 4> normalize_rgba(const std::array<unsigned char, 4>& rgba)
	{
		std::array<float, 4> rs;
		for (int i = 0; i != 4; i++) rs[i] = rgba[i] / 255.f;

		return rs;
	};


	constexpr auto print_name = [](auto&& arg)
	{return arg._name;};
	
	template<typename... Callables>
	struct visitor: Callables... { using Callables::operator()...;};

	inline std::array<int, 4> clamp_rgb(const std::array<int, 4>& rgba)
	{
		std::array<int,4> rs;
		std::ranges::transform(rgba, rs.begin(), [](int v){return std::clamp(v, 0, 255);});
		return rs;
	}

	inline std::array<float, 4> blend_rgba_additive(const std::array<float, 4>& c1, const std::array<float ,4>& c2)
	{
		std::array<float, 4> rs;

		rs[3] = 1 - (1 - c1[3]) * (1 - c2[3]);
		
		rs[2] = c2[2] * c2[3]/rs[3] + c1[2] * c1[3] * (1 - c2[3])/rs[3];
		rs[1] = c2[1] * c2[3]/rs[3] + c1[1] * c1[3] * (1 - c2[3])/rs[3];
		rs[0] = c2[0] * c2[3]/rs[3] + c1[0] * c1[3] * (1 - c2[3])/rs[3];
		
		return rs;
	};

	inline std::array<float, 4> blend_rgba_multiply(const std::array<float, 4>& c1, const std::array<float ,4>& c2)
	{
		std::array<float, 4> rs;

		rs[0] = c1[0] * c2[0];
		rs[1] = c1[1] * c2[1];
		rs[2] = c1[2] * c2[2];
		rs[3] = c1[3] * c2[3];
				
		return rs;
	};
	
	inline std::array<float, 4> blend_rgba_prealphamultiply(const std::array<float, 4>& c1, const std::array<float ,4>& c2)
	{
		std::array<float, 4> rs;
		
		rs[0] = c1[0] * (1 - c2[3]) + c2[0] * c2[3];
		rs[1] = c1[1] * (1 - c2[3]) + c2[1] * c2[3];
		rs[2] = c1[2] * (1 - c2[3]) + c2[2] * c2[3];
		rs[3] = c1[3] * (1 - c2[3]) + c2[3];
				
		return rs;
	};

	constexpr color lerp_rgba(const color& a, const color& b, float time)
	{
		return
		{
			static_cast<unsigned char>(a[0] + (b[0] - a[0]) * time),
			static_cast<unsigned char>(a[1] + (b[1] - a[1]) * time),
			static_cast<unsigned char>(a[2] + (b[2] - a[2]) * time),
			static_cast<unsigned char>(a[3] + (b[3] - a[3]) * time),
		};
	};


	template<std::size_t... Bits, std::size_t N>
	constexpr bool check_bits(const std::bitset<N>& bitset)
	{
		return (bitset.test(Bits) && ...);
	};

	template<std::size_t... Bits, std::size_t N>
	void set_bits(std::bitset<N>& bitset)
	{
		(bitset.set(Bits),...);
	};
	
	template<std::size_t... Bits, std::size_t N>
	void reset_bits(std::bitset<N>& bitset)
	{
		(bitset.reset(Bits),...);
	};

	inline std::vector<std::vector<std::string>> parse_tsv(const std::filesystem::path& file)
	{
		std::vector<std::vector<std::string>> rs;
		std::ifstream f {file};

		std::string line;
		while (std::getline(f,line))
		{
			std::vector<std::string> fields;
			while (line.find_first_of('\t') != std::string::npos)
			{
				auto tpos = line.find_first_of('\t');
				fields.emplace_back(line.substr(0, tpos));
				line = line.substr(tpos + 1);
			};
			rs.push_back(fields);
		};

		return rs;
	};


	constexpr float deg_to_rad(float degree)
	{
		return degree * std::numbers::pi / 180.f;
	};

	template<Numeric T>
	constexpr mmath::ivec2 id_at(const mmath::vec2<T>& v, int tilesize)
	{
		int idx = v.x / tilesize;
		int idy = v.y / tilesize;

		return {idx, idy};
	};

	inline mmath::irect rect_at(int index, int width, int height, int srcwidth, int srcheight)
	{
		int numx = srcwidth / width;
		int numy = srcheight / height;
		auto div = std::div(index, numx);
		if (div.quot >= numy) throw std::out_of_range {std::format("invalid id {}, exceed height of tile resource {}", index, numy)};
		
		return mmath::irect
		{
			{div.rem * width, div.quot * height},
			{width, height}
		};
	};




};

#endif
