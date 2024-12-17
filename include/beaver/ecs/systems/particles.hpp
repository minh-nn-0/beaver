#pragma once

#include <beaver/ecs/components/particles.hpp>

#include <random>
namespace beaver::system::particle
{
	using namespace component;
	
	inline void emit(particle_emitter& pe)
	{
			
	};

	void add_particle(particle_emitter& pe)
	{
		std::random_device rd;
		std::mt19937 re(rd());
		std::uniform_real_distribution<float> posx_dist(0.f, pe._area.x);
		std::uniform_real_distribution<float> posy_dist(0.f, pe._area.y);
		std::uniform_real_distribution<float> speed_dist(pe._speed_variation.x, pe._speed_variation.y);
		std::uniform_real_distribution<float> size_dist(pe._size_variation.x, pe._size_variation.y);
		std::uniform_real_distribution<float> spread_dist(-pe._spread / 2.f, pe._spread / 2.f);
		float spread_angle = spread_dist(re) + pe._direction;

		float size = size_dist(re);
		std::array<unsigned char, 4> pcolor = {255,255,255,255};

		position pos = {pe._emitting_position.x + posx_dist(re), 
						pe._emitting_position.y + posy_dist(re)};

		velocity vel = {speed_dist(re) * std::cos(spread_angle),
						speed_dist(re) * std::sin(spread_angle)};
		timing::countdown lifetime {pe._lifetime};
		auto new_particle = pe._particles.add_entity();

		pe._particles.set_component<position>(new_particle, pos);
		pe._particles.set_component<velocity>(new_particle, vel);
		pe._particles.set_component<scale>(new_particle, scale{size, size});
		pe._particles.set_component<color>(new_particle, color{pcolor});
		pe._particles.set_component<timing::countdown>(new_particle, lifetime);

	};

	void update(particle_emitter& pe, float dt)
	{
		if (pe._emitting)
		{
			pe._timer += dt;
			
			if (pe._timer >= 1.f/pe._rate) add_particle(pe);
		};
		


	};

	void stop(particle_emitter& pe)
	{
	};
};
