#ifndef M_INPUT_H
#define M_INPUT_H

#include <unordered_map>
#include <bitset>
#include <sdlwrapper/sdlwrapper.hpp>

namespace beaver
{
	//BUTTON HANDLING
	constexpr unsigned BTNU = 0;
	constexpr unsigned BTND = 1;
	constexpr unsigned BTNL = 2;
	constexpr unsigned BTNR = 3;
	constexpr unsigned BTNZ = 4;
	constexpr unsigned BTNX = 5;
	constexpr unsigned BTNC = 6;
	constexpr unsigned BTNV = 7;
	constexpr unsigned BTNO = 8;
	constexpr unsigned BTNE = 9;
	
	// Controller
	//
	struct controller
	{
		std::unordered_map<unsigned, unsigned> _keymap = {
			{SDLK_UP, 0},
			{SDLK_DOWN, 1},
			{SDLK_LEFT, 2},
			{SDLK_RIGHT, 3},
			{SDLK_z, 4},
			{SDLK_x, 5},
			{SDLK_c, 6},
			{SDLK_v, 7},
			{SDLK_q, 8},
			{SDLK_ESCAPE, 9}
		};
		void swap() {_keystate.second = _keystate.first;};	
		void update(const SDL_Event& e) 
		{
			// process input
			if (e.type == SDL_KEYDOWN)
				if (_keymap.contains(e.key.keysym.sym))
					_keystate.first.set(_keymap.at(e.key.keysym.sym));
			if (e.type == SDL_KEYUP)
				if (_keymap.contains(e.key.keysym.sym))
					_keystate.first.reset(_keymap.at(e.key.keysym.sym));
		};

		bool pressed(unsigned id) const {return _keystate.first.test(id);};
		bool just_pressed(unsigned id) const {return _keystate.first.test(id) && ! _keystate.second.test(id);};
		
		std::pair<std::bitset<10>, std::bitset<10>> _keystate;

	};
};
#endif
