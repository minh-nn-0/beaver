#ifndef M_FSM_H
#define M_FSM_H

#include <functional>
#include <unordered_map>

namespace beaver
{
	// finite state machine. Typically used with enum, number or string.
	// use std::unordered_map, so if need to use with custom type, 
	// we alse need custom hash and comparison function.
	template<std::equality_comparable T>
	struct fsm
	{
		void update()
		{
			_prev = _cur;
		};

		void new_transition(T from, T to, const std::function<void()>& f)
		{
			_transition_table[from][to] = f;
		};
		
		void set(T to)
		{
			if (to != _cur)
			{
				if (_transition_table.at(_cur).contains(to))
					_transition_table.at(_cur).at(to)();
				
				_cur = to;
			}
		};
		
		bool is(T s) const {return _cur == s;};
		bool was(T s) const {return _prev == s;};
		
		using transition = std::unordered_map<T, std::function<void()>>;
		
		std::unordered_map<T, transition> _transition_table;
		T _prev, _cur;

		
	};
	
};

#endif
