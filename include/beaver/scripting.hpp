#ifndef BEAVER_SCRIPTING_H
#define BEAVER_SCRIPTING_H

#include <sol/sol.hpp>
#include <mmath/core.hpp>
#include <beaver/sdlgame.hpp>
#include <beaver/graphics.hpp>
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
// ================= Mainly dealing with table in Lua ================ 

namespace beaver
{
	using namespace component;
	inline mmath::irect table_to_irect(const sol::table& tbl)
	{
		return {
			tbl.get_or("x", 0), tbl.get_or("y",0),
			tbl.get_or("w", 0), tbl.get_or("h",0)
		};
	};
	inline mmath::frect table_to_frect(const sol::table& tbl)
	{
		return {
			tbl.get_or("x", 0.f), tbl.get_or("y",0.f),
			tbl.get_or("w", 0.f), tbl.get_or("h",0.f)
		};
	};
	inline sol::table rect_to_table(const mmath::frect& rect)
	{
		sol::table rs;
		rs["x"] = rect._pos.x;
		rs["y"] = rect._pos.x;
		rs["w"] = rect._pos.x;
		rs["h"] = rect._pos.x;
		return rs;
	};

	void init_lua(sol::state& lua);
	void bind_core(beaver::sdlgame&, sol::state& lua);

	// ========================== ENTITY MANAGER
	template<typename... Ts>
	void bind_entity(beaver::ecs<Ts...>& ecs, sol::table& tbl)
	{
		tbl.set_function("add_entity", [&]{return ecs.add_entity();});
		tbl.set_function("entities_size", [&]{return ecs.size();});
		tbl.set_function("get_active_entities", [&] -> std::vector<std::size_t> {return ecs.active_entities();});
		tbl.set_function("set_active",[&](std::size_t eid, bool active) {ecs.set_active(eid, active);});
		tbl.set_function("is_active", [&](std::size_t eid) {return ecs.is_active(eid);});
		tbl.set_function("reset_entity", [&](std::size_t eid) {ecs.reset_entity(eid);});
		tbl.set_function("add_tag", [&](std::size_t eid, const std::string& tag) {ecs.add_tag(eid, tag);});
		tbl.set_function("remove_tag", [&](std::size_t eid, const std::string& tag) {ecs.remove_tag(eid, tag);});
		tbl.set_function("has_tag", [&](std::size_t eid, const std::string& tag) {return ecs.has_tag(eid, tag);});
		tbl.set_function("get_entities_with_tags", [&](const sol::table& tags) -> std::vector<std::size_t>
				{
					std::vector<std::string> tags_list;
					for (const auto& pair: tags) tags_list.push_back(pair.second.as<std::string>());
					return ecs.get_with_tags(tags_list);
				});
	};
	

	// ============================ SPATIAL COMPONENTS
	template<typename... Ts>
	void bind_position(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_position", [&](std::size_t eid) -> sol::table
		{
			auto& pos= ecs.template get_component<position>(eid);
			if (!pos.has_value()) return sol::nil;
			return lua.create_table_with(
					"x", pos->_value.x,
					"y", pos->_value.y);
		});
		tbl.set_function("set_position", [&](std::size_t eid, float x, float y)
				{
					ecs.template get_or_set_component<position>(eid)->_value = {x,y}; 
				});
		tbl.set_function("unset_position", [&](std::size_t eid)
				{
					ecs.template reset_component<position>(eid);
				});
	};

	template<typename... Ts>
	void bind_velocity(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_velocity", [&](std::size_t eid) -> sol::table
		{
			auto& vel= ecs.template get_component<velocity>(eid);
			if (!vel.has_value()) return sol::nil;
			return lua.create_table_with(
					"x", vel->_value.x,
					"y", vel->_value.y);
		});
		tbl.set_function("set_velocity", [&](std::size_t eid, float x, float y)
				{
					ecs.template get_or_set_component<velocity>(eid)->_value = {x,y}; 
				});
		tbl.set_function("unset_velocity", [&](std::size_t eid)
				{
					ecs.template reset_component<velocity>(eid);
				});
	};

	template<typename... Ts>
	void bind_scale(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_scale", [&](std::size_t eid) -> sol::table
		{
			auto& scale = ecs.template get_component<component::scale>(eid);
			if (!scale.has_value()) return sol::nil;
			return lua.create_table_with(
					"x", scale->_value.x,
					"y", scale->_value.y);
		});
		tbl.set_function("set_scale", [&](std::size_t eid, float x, float y)
				{
					ecs.template get_or_set_component<scale>(eid)->_value = {x,y}; 
				});
		tbl.set_function("unset_scale", [&](std::size_t eid)
				{
					ecs.template reset_component<scale>(eid);
				});
	};

	template<typename... Ts>
	void bind_rotation(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_rotation", [&](std::size_t eid) -> float
		{
			if (auto& rotation = ecs.template get_component<component::rotation>(eid); rotation.has_value())
				return rotation->_value;
			else return 0.f;
		});
		tbl.set_function("set_rotation", [&](std::size_t eid, float value)
				{
					ecs.template get_or_set_component<rotation>(eid)->_value = value; 
				});
		tbl.set_function("unset_rotation", [&](std::size_t eid)
				{
					ecs.template reset_component<rotation>(eid);
				});
	};

	template<typename... Ts>
	void bind_flipflag(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("set_flipflag", [&](std::size_t eid, unsigned flipflag)
				{
					ecs.template get_or_set_component<component::flipflag>(eid)->_value = flipflag;
				});
		tbl.set_function("get_flipflag", [&](std::size_t eid) -> unsigned
				{
					if (auto& flipflag = ecs.template get_component<component::flipflag>(eid); flipflag.has_value())
						return flipflag->_value;
					else return 0;
				});
	};

	template<typename... Ts>
	void bind_pivot(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_pivot", [&](std::size_t eid) -> sol::table
		{
			auto& pivot = ecs.template get_component<component::pivot>(eid);
			if (!pivot.has_value()) return sol::nil;
			return lua.create_table_with(
					"x", pivot->_value.x,
					"y", pivot->_value.y);
		});
		tbl.set_function("set_pivot", [&](std::size_t eid, float x, float y)
				{
					ecs.template get_or_set_component<pivot>(eid)->_value = {x,y}; 
				});

	};
	template<typename... Ts>
	void bind_cbox(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_cbox", [&](std::size_t eid) -> sol::table
				{
					if (auto& cbox = ecs.template get_component<aabb>(eid); cbox.has_value())
						return lua.create_table_with("x", cbox->_value._pos.x,
													"y", cbox->_value._pos.y,
													"w", cbox->_value._size.x,
													"h", cbox->_value._size.y);
					else return sol::nil;
				});
		tbl.set_function("set_cbox", sol::overload(
				[&](std::size_t eid, float x, float y, float w, float h)
				{
					ecs.template set_component<aabb>(eid, {._value = mmath::frect {x,y,w,h}});
				},
				[&](std::size_t eid, const sol::table& rect)
				{
					mmath::frect box {rect["x"], rect["y"], rect["w"], rect["h"]};
					ecs.template set_component<aabb>(eid, {._value = box});
				}));
		tbl.set_function("unset_cbox", [&](std::size_t eid)
				{
					ecs.template reset_component<aabb>(eid);
				});
	};
	// ========================= COLOR TINT
	template<typename... Ts>
	void bind_color(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_tint", [&](std::size_t eid) -> sol::table
				{
					if (auto& cl = ecs.template get_component<color>(eid); cl.has_value())
						return lua.create_table_with("r", cl->_value[0],
													"g", cl->_value[1],
													"b", cl->_value[2],
													"a", cl->_value[3]);
					else return sol::nil;
				});

		tbl.set_function("set_tint", sol::overload(
				[&](std::size_t eid, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
				{
					ecs.template set_component<color>(eid, color{._value {r,g,b,a}});
				},
				[&](std::size_t eid, const sol::table& param)
				{
					ecs.template set_component<color>(eid, color{._value {param["r"],param["g"],param["b"],param["a"]}});
				}));
	};

	// ======================== IMAGE RENDER
	template<typename... Ts>
	void bind_image_render(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("set_image", [&](std::size_t eid, const std::string& img_name)
				{
					ecs.template get_or_set_component<image_render>(eid)->_image_name = img_name;
				});
		tbl.set_function("set_image_source", [&](std::size_t eid, float x, float y, float w, float h)
				{
					ecs.template get_or_set_component<image_render>(eid)->_source = 
							mmath::frect {x,y,w,h};
				});
	};

	template<typename... Ts>
	void bind_entity_render(beaver::ecs<Ts...>& ecs, sdlgame& sdl, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("render_entity", [&](std::size_t eid)
				{
					auto& position = ecs.template get_component<component::position>(eid);
					auto& scale = ecs.template get_component<component::scale>(eid);
					auto& pivot = ecs.template get_component<component::pivot>(eid);
					auto& rotation = ecs.template get_component<component::rotation>(eid);
					auto& image_render = ecs.template get_component<component::image_render>(eid);
					auto& flip = ecs.template get_component<flipflag>(eid);

					if (!position.has_value() || !image_render.has_value()) return;

					const mmath::irect& src = image_render->_source;
					const auto* texture = sdl._assets.get<sdl::texture>(image_render->_image_name);
					mmath::frect dst {
						position->_value.x,
						position->_value.y,
						scale.has_value() ? src._size.x * scale->_value.x : src._size.x,
						scale.has_value() ? src._size.y * scale->_value.y : src._size.y
					};

					double angle = rotation.has_value() ? rotation->_value : 0.0;

					unsigned flipflags = flip.has_value() ? static_cast<SDL_RendererFlip>(flip->_value) : SDL_FLIP_NONE;
					mmath::fvec2 pv = pivot.has_value() ? pivot->_value : mmath::fvec2{0,0};
					sdl._graphics.texture(*texture, 
							dst, 
							src, 
							angle, 
							pv, 
							flipflags);
				});
	};

	template<typename... Ts>
	void bind_oscillation(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_oscillation", [&](std::size_t eid) -> sol::table
		{
			auto& osc= ecs.template get_component<oscillation>(eid);
			if (!osc.has_value()) return sol::nil;
			return lua.create_table_with(
				"center", lua.create_table_with("x", osc->_center.x, "y", osc->_center.y),
				"amplitude", osc->_amplitude,
				"frequency", osc->_frequency,
				"axis", lua.create_table_with("x", osc->_axis.x, "y", osc->_axis.y),
				"waveform", osc->_waveform
			);
		});

		tbl.set_function("set_oscillation", [&](std::size_t eid, sol::table params)
		{
			auto& osc = ecs.template get_or_set_component<oscillation>(eid);
			sol::table center_tbl = params["center"];
			osc->_center = {
				center_tbl["x"].get<float>(),
				center_tbl["y"].get<float>()
			};
			osc->_amplitude = params["amplitude"].get_or(osc->_amplitude);
			osc->_frequency = params["frequency"].get_or(osc->_frequency);
			sol::optional<sol::table> axis_tbl = params["axis"];
			if (axis_tbl.has_value())
				osc->_axis = {
					axis_tbl.value()["x"].get<float>(),
					axis_tbl.value()["y"].get<float>()
				};
			osc->_waveform = static_cast<oscillation::Waveform>(params["waveform"].get_or(osc->_waveform));
		});
	}


	// ========================= STRING FSM
	template<typename... Ts>
	void bind_state(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_state", [&](std::size_t eid) -> std::string
				{
					if (auto& fsm = ecs.template get_component<fsmstr>(eid); fsm.has_value())
						return fsm->_cur;
					else return "";
				});
		tbl.set_function("get_previous_state", [&](std::size_t eid) -> std::string
				{
					if (auto& fsm = ecs.template get_component<fsmstr>(eid); fsm.has_value())
						return fsm->_prev;
					else return "";
				});

		tbl.set_function("set_state", [&](std::size_t eid, const std::string& state)
				{
					ecs.template get_or_set_component<fsmstr>(eid)->set(state);
				});
		
		tbl.set_function("set_state_entry", 
				[&](std::size_t eid, const std::string& state, sol::function enter, sol::object exit)
				{
					ecs.template get_or_set_component<fsmstr>(eid)->add_state(
							state, [enter]{if (enter) enter();},
									[exit]{if (exit.is<sol::function>()) exit.as<sol::function>()();}
									);
				});
		tbl.set_function("set_state_transition", [&](std::size_t eid, 
					const std::string& from, const std::string& to, 
					sol::function transition)
				{
					ecs.template get_or_set_component<fsmstr>(eid)->add_transition(
							from, to, 
							[transition]{if (transition) transition();});
				});

	};

	// ======================== TILE_ANIMATION
	template<typename... Ts>
	void bind_tile_animation(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_animation", [&](std::size_t eid) -> sol::table
			{
				if (auto& anim = ecs. template get_component<beaver::tile_animation>(eid); anim.has_value())
					return lua.create_table_with("currentid", anim->current_frame()._id,
												"playing", anim->_playing);
				else return sol::nil;
			});
		tbl.set_function("set_tileanimation", [&](std::size_t eid, const sol::table& param) 
				{
					auto& anim = ecs.template get_or_set_component<beaver::tile_animation>(eid);
					anim->_framewidth = param["framewidth"].get<int>();
					anim->_frameheight = param["frameheight"].get<int>();
					anim->_repeat = param["repeat"].get_or(true);

					beaver::tile_animation::framevec frames {};
					for (auto& frame: param["frames"].get<sol::table>())
						if (frame.second.is<sol::table>())
							frames.emplace_back(frame.second.as<sol::table>()[1],
												frame.second.as<sol::table>()[2]);
					anim->new_frames(frames);
				});
		tbl.set_function("set_animation_repeat", [&](std::size_t eid, bool repeat) 
				{
					ecs.template get_or_set_component<beaver::tile_animation>(eid)->_repeat = repeat;
				});
		tbl.set_function("set_animation_playing", [&](std::size_t eid, bool playing)
				{
					ecs.template get_component<beaver::tile_animation>(eid)->_playing = playing;
				});

	};
	
	// ================== TIMING
	template<typename... Ts>
	void bind_timer(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_timer", [&](std::size_t eid) -> sol::table
				{
					if (auto& timer = ecs.template get_component<beaver::timing::countdown>(eid); timer.has_value()) 
						return lua.create_table_with("limit", timer->_limit,
													"elapsed", timer->_elapsed,
													"running", timer->running());
					else return sol::nil;
				});


		tbl.set_function("set_timer", [&](std::size_t eid, float limit)
				{
					ecs.template get_or_set_component<beaver::timing::countdown>(eid)->reset(limit);
				});

	};
	
	template<typename... Ts>
	void bind_stopwatch(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("get_stopwatch", [&](std::size_t eid) -> sol::object
			{
				if (auto& sw = ecs.template get_component<beaver::timing::stopwatch>(eid); sw.has_value())
					return sol::make_object(lua, sw->_elapsed);
				else return sol::nil;
			});
	
		tbl.set_function("set_stopwatch", [&](std::size_t eid)
			{
				ecs.template get_or_set_component<beaver::timing::stopwatch>(eid)->reset();
			});
	};
	


	template<typename... Ts>
	void bind_particle(beaver::ecs<Ts...>& ecs, sol::table& tbl, sol::state& lua)
	{
		tbl.set_function("set_particle_emitter_config", [&](std::size_t eid, const sol::table& param)
				{
					auto& pm = ecs.template get_or_set_component<particle_emitter>(eid);

					particle_emitter::particle_config config {};
					const auto& oldconfig {pm->_config};
					
					config._emitting_position = {param["emitting_position"]["x"].get_or(oldconfig._emitting_position.x),
												param["emitting_position"]["y"].get_or(oldconfig._emitting_position.y)};

					auto colors = param["color_gradient"];
					if (colors.valid())
					{
						auto& color_config = config._colors._value;
						for (auto keyframe_param: colors.get<sol::table>())
						{
							auto time_param = keyframe_param.second.as<sol::table>()["time"];
							auto color_param = keyframe_param.second.as<sol::table>()["color"];

							if (!time_param.valid()) throw std::runtime_error(std::format("time properties not found in particle color keyframe config"));
							if (!color_param.valid()) throw std::runtime_error(std::format("color properties not found in particle color keyframe config"));

							float new_time = time_param.get<float>();
							color new_color;
							for (int i = 0; i != 4; i++)
							{
								new_color._value[i] = color_param.get<sol::table>()[i+1];
							};

							color_config.push_back(color_keyframe{new_time, new_color});
						};

						std::ranges::sort(color_config, [](auto&& color1, auto&& color2){return color1._time < color2._time;});
					}
					else config._colors = oldconfig._colors;
					

					config._linear_acceleration._value = {param["linear_acceleration"]["x"].get_or(oldconfig._linear_acceleration._value.x),
															param["linear_acceleration"]["y"].get_or(oldconfig._linear_acceleration._value.y)};

					config._speed_variation = {param["speed_variation"]["min"].get_or(oldconfig._speed_variation.x),
												param["speed_variation"]["max"].get_or(oldconfig._speed_variation.y)};

					config._size_variation = {param["size_variation"]["min"].get_or(oldconfig._size_variation.x),
												param["size_variation"]["max"].get_or(oldconfig._size_variation.y)};

					config._area = {param["area"]["x"].get_or(oldconfig._area.x),
									param["area"]["y"].get_or(oldconfig._area.y)};

					config._direction = param["direction"].get_or(oldconfig._direction); 	
					config._spread = param["spread"].get_or(oldconfig._spread); 	
					config._lifetime = param["lifetime"].get_or(oldconfig._lifetime); 	
					config._rate = param["rate"].get_or(oldconfig._rate); 	

					pm->config(config);
				});
		tbl.set_function("manual_emit_particles", [&](std::size_t eid, int number, float x, float y, const sol::table& param)
				{
					auto& pm = ecs.template get_or_set_component<particle_emitter>(eid);
					particle_emitter::particle_config config {};
					auto& oldconfig = pm->_config;
					
					auto colors = param["color_gradient"];
					if (colors.valid())
					{
						auto& color_config = config._colors._value;
						for (auto keyframe_param: colors.get<sol::table>())
						{
							auto time_param = keyframe_param.second.as<sol::table>()["time"];
							auto color_param = keyframe_param.second.as<sol::table>()["color"];

							if (!time_param.valid()) throw std::runtime_error(std::format("time properties not found in particle color keyframe config"));
							if (!color_param.valid()) throw std::runtime_error(std::format("color properties not found in particle color keyframe config"));

							float new_time = time_param.get<float>();
							color new_color;
							for (int i = 0; i != 4; i++)
							{
								new_color._value[i] = color_param.get<sol::table>()[i+1];
							};

							color_config.push_back(color_keyframe{new_time, new_color});
						};

						std::ranges::sort(color_config, [](auto&& color1, auto&& color2){return color1._time < color2._time;});
					}
					else config._colors = oldconfig._colors;
					

					config._linear_acceleration._value = {param["linear_acceleration"]["x"].get_or(oldconfig._linear_acceleration._value.x),
															param["linear_acceleration"]["y"].get_or(oldconfig._linear_acceleration._value.y)};

					config._speed_variation = {param["speed_variation"]["min"].get_or(oldconfig._speed_variation.x),
												param["speed_variation"]["max"].get_or(oldconfig._speed_variation.y)};

					config._size_variation = {param["size_variation"]["min"].get_or(oldconfig._size_variation.x),
												param["size_variation"]["max"].get_or(oldconfig._size_variation.y)};

					config._area = {param["area"]["x"].get_or(oldconfig._area.x),
									param["area"]["y"].get_or(oldconfig._area.y)};

					config._direction = param["direction"].get_or(oldconfig._direction); 	
					config._spread = param["spread"].get_or(oldconfig._spread); 	
					config._lifetime = param["lifetime"].get_or(oldconfig._lifetime); 	
					config._rate = param["rate"].get_or(oldconfig._rate); 	

					pm->manual_emit(number, x, y, config);
				});
		tbl.set_function("set_particle_emitter_auto", [&](std::size_t eid, bool isauto)
				{
					auto& pm = ecs.template get_component<particle_emitter>(eid);

					if (!pm.has_value()) return;
					pm->_auto = isauto;
				});

		tbl.set_function("get_particles", [&](std::size_t eid) -> std::vector<std::size_t>
				{
					auto& pm = ecs.template get_component<particle_emitter>(eid);
					if (!pm.has_value()) throw std::runtime_error(std::format("particle emitter not found for entity {}",eid));
					return pm->_particles.active_entities();
				});

		tbl.set_function("get_particle_lifetime", [&](std::size_t eid, std::size_t pid) -> std::pair<float, float>
				{
					auto& pm = ecs.template get_component<particle_emitter>(eid);
					if (!pm.has_value()) throw std::runtime_error(std::format("particle emitter not found for entity {}",eid));
					
					auto& lifetime = pm->_particles.template get_component<timing::countdown>(pid);

					return std::make_pair(lifetime->_elapsed, lifetime->_limit);
				});
		tbl.set_function("get_particle_color", [&](std::size_t eid, std::size_t pid) -> decltype(color::_value)
				{
					auto& pm = ecs.template get_component<particle_emitter>(eid);
					if (!pm.has_value()) throw std::runtime_error(std::format("particle emitter not found for entity {}",eid));
					return pm->_particles.template get_component<color>(pid)->_value;
				});
		tbl.set_function("get_particle_size", [&](std::size_t eid, std::size_t pid) -> std::pair<float,float>
				{
					auto& pm = ecs.template get_component<particle_emitter>(eid);
					if (!pm.has_value()) throw std::runtime_error(std::format("particle emitter not found for entity {}",eid));
					auto& scl = pm->_particles.template get_component<scale>(pid);

					return std::make_pair(scl->_value.x, scl->_value.y);
				});
		tbl.set_function("set_particle_color", [&](std::size_t eid, std::size_t pid, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
				{
					auto& pm = ecs.template get_component<particle_emitter>(eid);
					if (!pm.has_value()) return;
					pm->_particles.template get_component<color>(pid)->_value = std::array<unsigned char, 4> {r,g,b,a};
				});
		tbl.set_function("set_particle_size", [&](std::size_t eid, std::size_t pid, float size)
				{
					auto& pm = ecs.template get_component<particle_emitter>(eid);
					if (!pm.has_value()) return;
					pm->_particles.template get_component<scale>(pid)->_value.x = size;
				});
	};
	

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

	template<typename... Ts>
	void bind_ecs_core_components(beaver::ecs<Ts...>& ecs, sdlgame& sdl, sol::table& tbl, sol::state& lua)
	{
		bind_entity(ecs, tbl);
		bind_position(ecs, tbl, lua);
		bind_velocity(ecs, tbl, lua);
		bind_scale(ecs, tbl, lua);
		bind_rotation(ecs, tbl, lua);
		bind_pivot(ecs, tbl, lua);
		bind_color(ecs,tbl,lua);
		bind_oscillation(ecs, tbl, lua);
		bind_flipflag(ecs, tbl, lua);
		bind_tile_animation(ecs, tbl, lua);
		bind_cbox(ecs,tbl,lua);
		bind_state(ecs, tbl, lua);
		bind_image_render(ecs, tbl, lua);
		bind_entity_render(ecs, sdl, tbl, lua);
		bind_timer(ecs, tbl, lua);
		bind_stopwatch(ecs, tbl, lua);
		bind_particle(ecs, tbl, lua);
	};
};

#endif
