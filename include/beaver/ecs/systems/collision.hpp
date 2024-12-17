#pragma once

#include <beaver/ecs/entity.hpp>
#include <beaver/ecs/components/position.hpp>
#include <beaver/ecs/components/cbox.hpp>

namespace beaver::system::collision
{
	using namespace component;
	
	//TODO implement Quad tree 
	//TODO what if entity rotate (have a rotation component) ?
	template<typename... Ts>
	std::vector<std::size_t> find_collisions(std::size_t eid, beaver::ecs<Ts...>& entities)
	{
		std::vector<std::size_t> rs;
		if (!(entities.template has_component<position>(eid) && entities.template has_component<aabb>(eid)))
			return rs;
		
		auto& posA = entities.template get_component<position>(eid);
		auto& cboxA = entities.template get_component<aabb>(eid);
		mmath::frect rectA = {posA->_value.x + cboxA->_value._pos.x,
							posA->_value.y + cboxA->_value._pos.y,
							cboxA->_value._size.x,
							cboxA->_value._size.y};
		for (std::size_t i = 0; i!= entities.size(); i++)
		{
			if (i != eid && entities.is_active(i) && entities.template has_component<position>(i) && entities.template has_component<aabb>(i))
			{
				auto& posB = entities.template get_component<position>(i);
				auto& cboxB = entities.template get_component<aabb>(i);
				mmath::frect rectB = {posB->_value.x + cboxB->_value._pos.x,
									posB->_value.y + cboxB->_value._pos.y,
									cboxB->_value._size.x,
									cboxB->_value._size.y};

				if (mmath::rect_intersecting(rectA, rectB)) rs.push_back(i);
			};
		};

		return rs;
	};
};

