#ifndef BEAVER_ENTITY_H
#define BEAVER_ENTITY_H

#include <tuple>
#include <set>
#include <string>
#include <stdexcept>
#include <format>
#include <vector>
#include <algorithm>
#include <ranges>

#ifndef NDEBUG
#include <cassert>
#endif

namespace beaver
{
namespace entity
{
	template<typename T>
	concept Component = std::copy_constructible<T> && requires {std::is_trivial_v<T>;};

	struct entity
	{
		std::size_t	_id;
		bool _active;
		std::set<std::string>	_tags;
	};

	template<Component... Ts> 
	struct manager
	{
		manager() 
		{
		}

		std::tuple<std::vector<Ts>...> _component_storage;	
		std::vector<entity>		_entities;

		std::size_t	size() const {return _entities.size();};

		std::size_t add_entity()
		{
			auto replace_entity = [&](std::size_t id)
			{
				_entities.at(id)._active = true;
				_entities.at(id)._tags.clear();

				return id;
			};
			auto new_entity = [&]()
			{
				_entities.emplace_back(_entities.size(), true);
				(std::get<std::vector<Ts>>(_component_storage).emplace_back(Ts{}),...);

				return _entities.size() - 1;
			};

			std::size_t new_id;
			
			if (_entities.size() != 0)
			{
				auto inactive_entity = std::ranges::find_if(_entities, [](auto&& e){return e._active == false;});
				
				if (inactive_entity != _entities.end())
					new_id = replace_entity(inactive_entity->_id);
				else
					new_id = new_entity();
			}
			else
				new_id = new_entity();
			
			return new_id;
		};

		std::set<std::string> get_tags(std::size_t eid)
		{
			return _entities.at(eid)._tags;
		};

		template<Component T>
		[[nodiscard]] std::vector<T>& get_component_vec() 
		{
			return std::get<std::vector<T>>(_component_storage);
		};
		
		// Get one component at eid
		template<Component T>
		T* get_component(std::size_t eid)
		{
			if (!_entities.at(eid)._active) throw std::runtime_error(std::format("get component failed, entity at {} is inactive", eid));
			
			return &get_component_vec<T>().at(eid);
		};

		// Get some components at eid
		template<Component... CompTs>
		[[nodiscard]] std::tuple<CompTs*...> get_components(std::size_t eid)
		{
			std::tuple<CompTs*...> rs;
			((std::get<CompTs*>(rs) = get_component<CompTs>(eid)),...);

			return rs;
		};

		// Get all components at eid
		[[nodiscard]] std::tuple<Ts*...> get_components(std::size_t eid)
		{
			std::tuple<Ts*...> rs;
			((std::get<Ts*>(rs) = get_component<Ts*>(eid)),...);

			return rs;
		};

		// View into some components
		template<Component... CompTs>
		[[nodiscard]] std::vector<std::tuple<CompTs*...>> view()
		{
			std::vector<std::tuple<CompTs*...>> rs;

			for (std::size_t i = 0; i != size(); i++)
			{
				if (_entities.at(i)._active)
				{
					std::tuple<CompTs*...> tup;
					
					((std::get<CompTs*>(tup) = get_component<CompTs>(i)), ...);
					
					rs.push_back(tup);
				};
			};

			return rs;
		};
	};
};
};

#endif
