#pragma once

#include <beaver/ecs/entity.hpp>
#include <beaver/ecs/components/tileanimation.hpp>
#include <beaver/ecs/components/render.hpp>

#include <beaver/sdlgame.hpp>
#include <utilities.hpp>
namespace beaver::system::animation
{
	using namespace component;
	template<typename... Ts>
	void update_tile_animation(beaver::assets_manager& assets, beaver::ecs<Ts...>& entities, float dt)
	{
		for (auto&& eid: entities.template with<tile_animation>())
		{
			auto& anim = entities.template get_component<tile_animation>(eid);
			anim->update(dt);

			auto& img_render = entities.template get_component<image_render>(eid);
			if (!img_render.has_value())
				std::cerr << "eid " << eid << "have tile animation but didn't set image" << std::endl;
			else
			{
				sdl::texture* img = assets.get<sdl::texture>(img_render->_image_name);
				img_render->_source = utils::rect_at(
									anim->current_frame()._id,
									anim->_framewidth,
									anim->_frameheight,
									img->_width,
									img->_height);
			}
		};
							
	};
};
