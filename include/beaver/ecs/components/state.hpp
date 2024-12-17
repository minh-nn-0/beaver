#ifndef M_FSM_H
#define M_FSM_H

#include <functional>
#include <unordered_map>
#include <string>
namespace beaver::component
{
	// finite state machine. Typically used with enum, number or string.
	// use std::unordered_map, so if need to use with custom type, 
	// we alse need custom hash and comparison function.
	template<typename T>
	struct fsm
	{
		using transition = std::unordered_map<T, std::function<void()>>;
		struct state_actions
		{
			std::function<void()> _enter = {};
			std::function<void()> _exit = {};
			transition _transitions;
		};

		std::unordered_map<T, state_actions> _state_table;
		
		void update()
		{
			_prev = _cur;
		};

		void add_state(const T& state, 
				const std::function<void()>& enter = {},
				const std::function<void()>& exit = {})
		{
			auto& state_entry = _state_table[state];
			state_entry._enter = enter;
			state_entry._exit = exit;
		};

		void add_transition(const T& from, const T& to, const std::function<void()>& f)
		{
			_state_table[from]._transitions[to] = f;
		};
		
		void set(const T& to)
		{
			if (to != _cur)
			{
				auto cur_state = _state_table.find(_cur);
				auto to_state = _state_table.find(to);

				if (cur_state != _state_table.end())
				{
					if (cur_state->second._exit)
						cur_state->second._exit();

					auto transition_it = cur_state->second._transitions.find(to);
					if (transition_it != cur_state->second._transitions.end())
						transition_it->second();
				};

				if (to_state != _state_table.end() && to_state->second._enter)
					to_state->second._enter();

				_prev = _cur;
				_cur = to;
			}
		};

		void set_if(const T& to, const std::function<bool()>& pred)
		{
			if (pred && pred()) set(to);
		};
		
		bool is(const T& s) const {return _cur == s;};
		bool was(const T& s) const {return _prev == s;};
		
		
		T _prev, _cur;

		
	};
	
	using fsmstr = fsm<std::string>;
};

#endif
