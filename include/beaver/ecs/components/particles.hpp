#ifndef BEAVER_PARTICLES_H
#define BEAVER_PARTICLES_H

#include <mmath/core.hpp>
#include <beaver/ecs/entity.hpp>
#include <beaver/ecs/components/position.hpp>
#include <beaver/ecs/components/velocity.hpp>
#include <beaver/ecs/components/scale.hpp>
#include <beaver/ecs/components/time.hpp>
#include <beaver/ecs/components/color.hpp>
#include <beaver/graphics.hpp>
#include <random>
namespace beaver::component
{
	// emit circle
	struct particle_emitter
	{
		struct linear_acceleration
		{
			mmath::fvec2 _value;
		};

		struct color_gradient
		{
			std::vector<color_keyframe> _value;
		};
		struct particle_config
		{
			color_gradient _colors;
			linear_acceleration _linear_acceleration {0,0};
			mmath::fvec2 _emitting_position;
			mmath::fvec2 _speed_variation {10,10};
			mmath::fvec2 _size_variation {1,1};
			mmath::fvec2 _area {0,0};
			float _direction {utils::deg_to_rad(270)}; // rad
			float _spread {utils::deg_to_rad(20)}; // rad
			float _lifetime {1};
			int _rate {5};
		};

		// scale have 2 value, one for current size (x), one for initial size (y)
		beaver::ecs<position, velocity, linear_acceleration, scale, timing::countdown, color, color_gradient> _particles;
		std::mt19937 _re {std::random_device{}()};
		particle_config _config;
		float _timer {0};
		bool _auto {false};
	
		color::value_t current_color_gradient(std::size_t pid);
		void config(const particle_config& config) {_config = config;};
		void manual_emit(int number, float x, float y, const particle_config& config); 
		void auto_emit(); 
		void update(float dt);
		void draw(beaver::graphics& grp);
	};
};

#endif
