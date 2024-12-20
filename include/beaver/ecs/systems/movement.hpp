#pragma once

#include <beaver/ecs/entity.hpp>
#include <beaver/ecs/components/position.hpp>
#include <beaver/ecs/components/velocity.hpp>
#include <beaver/ecs/components/oscillation.hpp>
#include <beaver/ecs/components/time.hpp>
namespace beaver::system::movement
{
	using namespace component;
	constexpr position apply_velocity(const position& pos, const velocity& vel, float dt)
	{
		return {._value = {pos._value.x + vel._value.x * dt, pos._value.y + vel._value.y * dt}};
	};

	constexpr velocity apply_gravity(const velocity& vel, float gravity, float dt)
	{
		return {vel._value.x, vel._value.y + gravity * dt};
	};
	
	constexpr position apply_oscillation(const oscillation& osc)
	{
		float omega = 2 * std::numbers::pi * osc._frequency;
		float oscillation_value = 0;
		// Calculate oscillation value based on the waveform type
		switch (osc._waveform) 
		{
			case oscillation::SINUSOIDAL:
				oscillation_value = osc._amplitude * std::sin(omega * osc._timer);
				break;

			case oscillation::TRIANGULAR:
				// For triangular wave, we use a linear ramp based on osc._timer modulated by frequency
				oscillation_value = osc._amplitude * (2.0f / std::numbers::pi) * std::asin(std::sin(omega * osc._timer));
				break;

			case oscillation::SQUARE:
				// For square wave, use a simple sign function to alternate between +amplitude and -amplitude
				oscillation_value = (std::sin(omega * osc._timer) >= 0.0f) ? osc._amplitude : -osc._amplitude;
				break;
		}

		return {osc._center.x + oscillation_value * osc._axis.x,
				osc._center.y + oscillation_value * osc._axis.y};
	};

	template<typename... Ts>
	void update(beaver::ecs<Ts...>& entities, float gravity, float dt)
	{
		std::ranges::for_each(entities.template with<position, velocity>(), 
				[&](auto&& eid)
			{
				position new_pos = entities.template get_component<position>(eid).value();
				velocity new_vel = entities.template get_component<velocity>(eid).value();


				if (!entities.has_tag(eid, "no_gravity"))
					new_vel = apply_gravity(new_vel, gravity, dt);

				if (entities.template has_component<oscillation>(eid))
				{
					oscillation& osc = entities.template get_component<oscillation>(eid).value();
					osc._timer += dt;

					new_pos = apply_oscillation(osc);
				};

				new_pos = apply_velocity(new_pos, new_vel, dt);

				entities.template set_component<position>(eid, new_pos);
				entities.template set_component<velocity>(eid, new_vel);
			});
	};
};
