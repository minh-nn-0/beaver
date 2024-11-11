#ifndef M_UTILITIES_H
#define M_UTILITIES_H
#include <sstream>
#include <array>
#include <algorithm>
#include <bitset>

namespace utils
{
	inline std::array<int,4> hex_to_rgba(const std::string& hex) {
		std::stringstream ss;
		std::string hex_str = hex.substr(hex[0] == '#' ? 1 : 0);
		if (hex_str.length() != 6 && hex_str.length() != 8) 
			throw std::invalid_argument("bad argument, incorrect format for hex");
		
		std::array<int,4> rgba {0,0,0,255};
		for (int i = 0; i != hex_str.length()/2; i++)
		{
			ss.clear();
			ss << hex_str.substr(i*2,2);
			ss >> std::hex >> rgba[i];
		};
		return rgba;
	}

	inline std::array<float, 4> normalize_rgba(const std::array<int, 4>& rgba)
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


};

#endif
