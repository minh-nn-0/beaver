#include <beaver/ecs/components/particles.hpp>
#include <utilities.hpp>
#include <random>
void beaver::particles::manager::emit(int number, const mmath::fvec2& position)
{
	std::random_device rd;
	std::mt19937 re(rd());
	std::uniform_real_distribution<float> posx_dist(0.f, _area.x);
	std::uniform_real_distribution<float> posy_dist(0.f, _area.y);
	std::uniform_real_distribution<float> speed_dist(_props._speed_variation.x, _props._speed_variation.y);
	std::uniform_real_distribution<float> size_dist(_props._size_variation.x, _props._size_variation.y);
	std::uniform_real_distribution<float> spread_dist(-_props._spread / 2.f, _props._spread / 2.f);
	
	for (int i = 0; i!= number; i++)
	{
		float spread_angle = utils::deg_to_rad(spread_dist(re) + _props._direction);

		float size = size_dist(re);
		_particles.push_back({._color = {255,255,255,255},
							  ._position = {position.x + posx_dist(re), position.y + posy_dist(re)},
							  ._velocity = {speed_dist(re) * std::cos(spread_angle), speed_dist(re) * std::sin(spread_angle)},
							  ._size = size,
							  ._start_size = size,
							  ._lifetime = _props._lifetime
							  });

	};
};

void beaver::particles::manager::update(float dt)
{
	for (auto& particle: _particles)
	{
		particle._velocity += _props._linear_acceleration * dt;
		particle._position += particle._velocity * dt;

		float lifetime_ratio = particle._lifetime / _props._lifetime;
		
		particle._size = lifetime_ratio * particle._start_size;
		
		particle._lifetime -= dt;
	};

	auto [to_remove_begin, to_remove_end] = std::ranges::remove_if(_particles,
			[](auto&& particle){return particle._lifetime <= 0;});

	_particles.erase(to_remove_begin, to_remove_end);
}

void beaver::particles::manager::draw(beaver::graphics& grp)
{
	for (auto& particle: _particles)
	{
		grp.set_draw_color(particle._color);
		grp.circle(particle._position.x, particle._position.y, particle._size, true);
		grp.set_draw_color({255,255,255,255});
	};
};
