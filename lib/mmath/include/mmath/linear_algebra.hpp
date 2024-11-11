#ifndef NNM_LNA_H
#define NNM_LNA_H

#include <iostream>
#include <cmath>


template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;
namespace mmath
{
	//TODO make a generic vector type. so we can write ivec<2> vec1, vec1.at()....
	template<Numeric UnitT>
	struct vec2
	{
		UnitT x,y;

		float size() const
		{
			return std::sqrt(x*x + y*y);
		};

		vec2 operator + (const vec2&v) const
		{
			return vec2{.x = x + v.x, .y = y + v.y};
		}
		vec2& operator += (const vec2&v) 
		{
			x += v.x ; y += v.y;
			return *this;
		}
		vec2 operator - (const vec2&v) const
		{
			return vec2{.x = x - v.x, .y = y - v.y};
		}
		vec2& operator -= (const vec2&v)
		{
			x -= v.x; y -= v.y;
			return *this;
		};

		vec2 operator * (const vec2&v) const
		{
			return vec2{.x = x *v.x, .y = y * v.y};
		}
		vec2& operator *= (const vec2&v) 
		{
			x *= v.x; y *= v.y;
			return *this;
		}

		vec2 operator / (const vec2&v) const
		{
			return vec2{.x = x /v.x, .y = y /v.y};
		};
		
		vec2& operator /= (const vec2&v) const
		{
			x /= v.x; y /= v.y;
			return *this;
		};

		vec2<UnitT> operator * (UnitT scale) const
		{
			return vec2{.x = x * scale, .y = y * scale};
		};
		vec2<UnitT> operator / (UnitT scale) const
		{
			return vec2{.x = x / scale, .y = y / scale};
		};
		
		auto operator <=> (const vec2&) const = default;
		
		template<Numeric T>
		operator vec2<T>()
		{
			return vec2<T>
			{
				static_cast<T>(x),
				static_cast<T>(y),
			};
		};

		friend std::ostream& operator << (std::ostream& os, vec2& p)
		{
			return os << p.x << ", " << p.y;
		};
	};

	using ivec2 = vec2<int>;
	using fvec2 = vec2<float>;
}


#endif
