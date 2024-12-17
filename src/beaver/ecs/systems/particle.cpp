#include <beaver/ecs/components/particles.hpp>
#include <beaver/ecs/systems/movement.hpp>
using namespace beaver::component;
color::value_t particle_emitter::current_color_gradient(std::size_t pid)
{
	auto& life = _particles.get_component<timing::countdown>(pid).value();
	float progress = life.progress();
	auto& colors = _particles.get_component<color_gradient>(pid).value();
	for (std::size_t i = 0; i != colors._value.size(); i++)
	{
		const auto& kf1 = colors._value[i];
		const auto& kf2 = colors._value[i+1];
		if (progress >= kf1._time && progress <= kf2._time)
		{
			float t = (progress - kf1._time) / (kf2._time - kf1._time);
			return utils::lerp_rgba(kf1._color._value, kf2._color._value, t);
		};
	};
	return colors._value.back()._color._value;

};
void particle_emitter::auto_emit()
{
	std::uniform_real_distribution<float> positionx_dist{0.f, _config._area.x},
										  positiony_dist{0.f, _config._area.y},
										  speed_dist{_config._speed_variation.x, _config._speed_variation.y},
										  size_dist{_config._size_variation.x, _config._size_variation.y},
										  spread_dist{- _config._spread / 2.f, _config._spread/ 2.f};
	float spread_angle = spread_dist(_re) + _config._direction;


	float size = size_dist(_re);
	std::array<unsigned char, 4> pcolor = {255,255,255,255};

	position pos = {_config._emitting_position.x + positionx_dist(_re), 
					_config._emitting_position.y + positiony_dist(_re)};

	velocity vel = {speed_dist(_re) * std::cos(spread_angle),
					speed_dist(_re) * std::sin(spread_angle)};

	timing::countdown lifetime = {_config._lifetime};

	auto new_particle = _particles.add_entity();

	_particles.set_component<linear_acceleration>(new_particle, _config._linear_acceleration);
	_particles.set_component<position>(new_particle, pos);
	_particles.set_component<velocity>(new_particle, vel);
	_particles.set_component<scale>(new_particle, scale{size, size});
	_particles.set_component<color>(new_particle, color{pcolor});
	_particles.set_component<timing::countdown>(new_particle, lifetime);
	if (_config._colors._value.size() > 0) _particles.set_component<color_gradient>(new_particle, _config._colors);
};

void particle_emitter::manual_emit(int number, float x, float y, const particle_config& config)
{
	for (int i = 0; i!= number; i++)
	{
		std::uniform_real_distribution<float> positionx_dist{0.f, config._area.x},
											  positiony_dist{0.f, config._area.y},
											  speed_dist{config._speed_variation.x, config._speed_variation.y},
											  size_dist{config._size_variation.x, config._size_variation.y},
											  spread_dist{- config._spread / 2.f, config._spread/ 2.f};
		float spread_angle = spread_dist(_re) + config._direction;


		float size = size_dist(_re);

		color::value_t pcolor = {255,255,255,255};

		position pos = {x + positionx_dist(_re), 
						y + positiony_dist(_re)};

		velocity vel = {speed_dist(_re) * std::cos(spread_angle),
						speed_dist(_re) * std::sin(spread_angle)};

		timing::countdown lifetime = {config._lifetime};

		auto new_particle = _particles.add_entity();

		_particles.set_component<linear_acceleration>(new_particle, config._linear_acceleration);
		_particles.set_component<position>(new_particle, pos);
		_particles.set_component<velocity>(new_particle, vel);
		_particles.set_component<scale>(new_particle, scale{size, size});
		_particles.set_component<color>(new_particle, color{pcolor});
		_particles.set_component<timing::countdown>(new_particle, lifetime);

		if (config._colors._value.size() > 0) _particles.set_component<color_gradient>(new_particle, config._colors);
	};
};

void particle_emitter::update(float dt)
{
	if (_auto)
	{
		_timer += dt;
		
		float emission_rate = 1.f/_config._rate;
		while (_timer >= emission_rate)
		{
			auto_emit();
			_timer -= emission_rate;
		};
	};

	// basic movement and lifetime update
	for (auto& particle: _particles.active_entities())
	{
		auto& pos = _particles.get_component<position>(particle).value();
		auto& vel = _particles.get_component<velocity>(particle).value();
		auto& acc = _particles.get_component<linear_acceleration>(particle).value();
		auto& life = _particles.get_component<timing::countdown>(particle).value();
		auto& cl = _particles.get_component<color>(particle).value();

		vel._value += acc._value * dt;
		pos = system::movement::apply_velocity(pos, vel, dt);
		
		life.update(dt);
		if (_particles.has_component<color_gradient>(particle))
			cl._value = current_color_gradient(particle);

		if (!life.running()) _particles._entities.at(particle)._active = false;
	};

	// cleanup
	_particles.clear_inactive();
};

void particle_emitter::draw(beaver::graphics& grp)
{
	for (const auto& particle: _particles.active_entities())
	{
		auto& pos = _particles.get_component<position>(particle).value();
		auto& cl = _particles.get_component<color>(particle).value();
		auto& size = _particles.get_component<scale>(particle).value();
		beaver::graphics::color oldcolor = grp.get_draw_color();
		grp.set_draw_color(cl._value);

		grp.circle(pos._value.x, pos._value.y, size._value.x, true);

		grp.set_draw_color(oldcolor);
	};
};
