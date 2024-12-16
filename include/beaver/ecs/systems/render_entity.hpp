#pragma once

#include <beaver/ecs/entity.hpp>
#include <beaver/ecs/components/position.hpp>
#include <beaver/ecs/components/scale.hpp>
#include <beaver/ecs/components/rotation.hpp>
#include <beaver/ecs/components/pivot.hpp>
#include <beaver/ecs/components/flipflag.hpp>
#include <beaver/ecs/components/render.hpp>
#include <beaver/ecs/components/color.hpp>
#include <beaver/sdlgame.hpp>
#include <utilities.hpp>

namespace beaver::system::render
{
	using namespace component;

	//TODO: Make it generic, we want point, circle,... too. Maybe a rendertype component or something
	template<typename... Ts>
	void render_entity(std::size_t eid, beaver::ecs<Ts...>& entities, beaver::sdlgame& game)
	{
		auto& position = entities.template get_component<component::position>(eid);
		auto& scale = entities.template get_component<component::scale>(eid);
		auto& pivot = entities.template get_component<component::pivot>(eid);
		auto& rotation = entities.template get_component<component::rotation>(eid);
		auto& image_render = entities.template get_component<component::image_render>(eid);
		auto& flip = entities.template get_component<component::flipflag>(eid);
		auto& cl = entities.template get_component<color>(eid);

		if (!position.has_value() || !image_render.has_value())
			return;

		const auto* texture = game._assets.get<sdl::texture>(image_render->_image_name);
		
		color oldcolor;
		SDL_GetTextureColorMod(*texture, &oldcolor._value[0], &oldcolor._value[1], &oldcolor._value[2]);
		SDL_GetTextureAlphaMod(*texture, &oldcolor._value[3]);
	
		if (cl.has_value())
		{
			SDL_SetTextureColorMod(*texture, cl->_value[0], cl->_value[1], cl->_value[2]);
			SDL_SetTextureAlphaMod(*texture, cl->_value[3]);
		};

		const mmath::irect& src = image_render->_source;
		mmath::frect dst {
			position->_value.x,
			position->_value.y,
			scale.has_value() ? src._size.x * scale->_value.x : src._size.x,
			scale.has_value() ? src._size.y * scale->_value.y : src._size.y
		};

		double angle = rotation.has_value() ? rotation->_value : 0.0;

		unsigned flipflags = flip.has_value() ? static_cast<SDL_RendererFlip>(flip->_value) : SDL_FLIP_NONE;
		mmath::fvec2 pv = pivot.has_value() ? 
				mmath::fvec2{pivot->_value.x * scale->_value.x,
							 pivot->_value.y * scale->_value.y}
				: mmath::fvec2{dst._size.x / 2, dst._size.y / 2};

		game._graphics.texture(*texture, 
				dst, 
				src, 
				angle, 
				pv, 
				flipflags);
		SDL_SetTextureColorMod(*texture, oldcolor._value[0], oldcolor._value[1], oldcolor._value[2]);
		SDL_SetTextureAlphaMod(*texture, oldcolor._value[3]);
	};
};
