#pragma once
#include <beaver/ecs/entity.hpp>
#include <beaver/ecs/components/position.hpp>
#include <beaver/ecs/components/velocity.hpp>
#include <beaver/ecs/components/scale.hpp>
#include <beaver/ecs/components/rotation.hpp>
#include <beaver/ecs/components/flipflag.hpp>
#include <beaver/ecs/components/pivot.hpp>
#include <beaver/ecs/components/cbox.hpp>
#include <beaver/ecs/components/render.hpp>
#include <beaver/ecs/components/oscillation.hpp>
#include <beaver/ecs/components/state.hpp>
#include <beaver/ecs/components/color.hpp>
#include <beaver/ecs/components/tileanimation.hpp>
#include <beaver/ecs/components/particles.hpp>
#include <beaver/ecs/components/time.hpp>

namespace beaver
{
	template<typename... Ts>
	using ecs_core = ecs<
						beaver::component::position,
						beaver::component::velocity,
						beaver::component::scale,
						beaver::component::rotation,
						beaver::component::pivot,
						beaver::component::color,
						beaver::component::oscillation,
						beaver::component::flipflag,
						beaver::component::image_render,
						beaver::component::tile_animation,
						beaver::component::aabb,
						beaver::component::particle_emitter,
						beaver::component::fsmstr,
						beaver::component::timing::countdown,
						beaver::component::timing::stopwatch,
						Ts...>;
};

