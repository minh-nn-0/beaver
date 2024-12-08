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
	concept Component = std::copy_constructible<T> && requires {std::is_default_constructible_v<T>;};
	

	template<Component... Ts>
	struct manager_v2
	{
		manager_v2() = default;
		struct entity
		{
			std::set<std::string>	_tags;
			std::size_t	_id;
			bool _active;
		};
		template<typename T>
		using vec_t = std::vector<std::optional<T>>;
		
		std::tuple<vec_t<Ts>...> 	_component_storage;	
		std::vector<entity>			_entities;
		std::vector<std::size_t>	_free_list;

		std::size_t size() const {return _entities.size();};
		
		void check_valid_entity(std::size_t eid)
		{
			if (eid >= size()) throw std::runtime_error(std::format("eid {} out of bound", eid));
		};

		std::size_t add_entity()
		{
			std::size_t new_id;
			
			if (!_free_list.empty())
			{
				new_id = _free_list.back();
				_free_list.pop_back();

				_entities.at(new_id)._active = true;
			}
			else
			{
				_entities.emplace_back(std::set<std::string>{}, _entities.size(), true);
				(view_one<Ts>()->emplace_back(std::nullopt),...);

				new_id = _entities.size() - 1;
			};

			return new_id;
		};

		void reset_entity(std::size_t eid)
		{
			check_valid_entity(eid);

			_entities.at(eid)._active = false;
			_entities.at(eid)._tags.clear();
			
			(reset_component<Ts>(eid),...);

			_free_list.push_back(eid);

		};
		void add_tags(std::size_t eid, const std::string& tag)
		{
			check_valid_entity(eid);

			if (!get_tags(eid).contains(tag))
				get_tags(eid).emplace(tag);
		};
		
		void remove_tags(std::size_t eid, const std::string& tag)
		{
			check_valid_entity(eid);

			if (get_tags(eid).contains(tag))
				get_tags(eid).erase(tag);

		};

		std::set<std::string>& get_tags(std::size_t eid)
		{
			check_valid_entity(eid);

			return _entities.at(eid)._tags;
		};

		template<Component T>
		std::optional<T>& add_component(std::size_t eid, const T& comp)
		{
			check_valid_entity(eid);

			auto& component = get_component<T>(eid);

			component.emplace(comp);
			return component;
		};

		template<Component T>
		void reset_component(std::size_t eid)
		{
			check_valid_entity(eid);

			get_component<T>(eid).reset();
		};

		template<Component T>
		std::optional<T>& get_component(std::size_t eid)
		{
			check_valid_entity(eid);

			return view_one<T>()->at(eid);
		};

		template<Component T>
		bool have_component(std::size_t eid)
		{
			check_valid_entity(eid);

			return get_component<T>(eid).has_value();
		};

		template<Component T>
		vec_t<T>* view_one()
		{
			return &std::get<vec_t<T>>(_component_storage);
		};

		template<Component... CompTs>
		std::tuple<vec_t<CompTs>*...> view()
		{
			return std::make_tuple(view_one<CompTs>()...);
		};

	};

	template<Component... Ts> 
	struct manager
	{
		manager() = default;

		struct entity
		{
			std::set<std::string>	_tags;
			std::size_t	_id;
			bool _active;
		};

		template<typename T>
		struct component
		{
			T _value;
			bool _active;
		};

		std::tuple<std::vector<component<Ts>>...> _component_storage;	
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
				_entities.emplace_back(std::set<std::string>{}, _entities.size(), true);
				(get_component_vec<Ts>().emplace_back(Ts{}),...);

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

		void remove_entity(std::size_t eid)
		{
			if (eid >= size() || !_entities.at(eid)._active) return;

			_entities.at(eid)._active = false;
			
			std::apply([eid](auto&... comps)
					{
						((comps.at(eid)._active = false),...);
					}, _component_storage);
		};

		void add_tags(std::size_t eid, const std::string& tag)
		{
			if (!get_tags(eid).contains(tag))
				get_tags(eid).emplace(tag);
		};
		
		void remove_tags(std::size_t eid, const std::string& tag)
		{
			if (get_tags(eid).contains(tag))
				get_tags(eid).erase(std::ranges::find(get_tags(eid), tag));

		};

		std::set<std::string>& get_tags(std::size_t eid)
		{
			return _entities.at(eid)._tags;
		};

		template<Component T>
		component<T>* add_component(std::size_t eid, const T& comp)
		{
			set_component<T>(eid, comp);

			return get_component<T>(eid);
		};

		template<Component T>
		void remove_component(std::size_t eid)
		{
			get_component<T>(eid)->_active = false;
		};
		template<Component... CompTs>
		bool have_components(std::size_t eid)
		{
			bool rs = true;
			std::apply([&](const auto&... comps)
					{
						((rs &= comps.at(eid)._active),...);
					}, _component_storage);
			return rs;
		};


		template<Component T>
		[[nodiscard]] std::vector<component<T>>& get_component_vec() 
		{
			return std::get<std::vector<component<T>>>(_component_storage);
		};
		
		// Get one component at eid
		template<Component T>
		component<T>* get_component(std::size_t eid)
		{
			if (!_entities.at(eid)._active) throw std::runtime_error(std::format("get component failed, entity at {} is inactive", eid));

			return &get_component_vec<T>().at(eid);
		};

		template<Component T>
		void set_component(std::size_t eid, const T& comp)
		{
			*get_component<T>(eid) = component<T>{comp, true};
		}

		// Get some components at eid
		template<Component... CompTs>
		[[nodiscard]] std::tuple<component<CompTs>*...> get_components(std::size_t eid)
		{
			std::tuple<component<CompTs>*...> rs;
			((std::get<component<CompTs>*>(rs) = get_component<CompTs>(eid)),...);

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

	template<entity::Component... Ts>
	using ecs = entity::manager_v2<Ts...>;
};

#endif
