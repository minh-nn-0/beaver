#ifndef M_INPUT_H
#define M_INPUT_H

#include <unordered_map>
#include <bitset>
#include <sdlwrapper/sdlwrapper.hpp>

namespace beaver
{
	struct controller
	{
		std::unordered_map<unsigned, int> _keystate;
		void update(const SDL_Event& e) 
		{
			// process input
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
				_keystate[e.key.keysym.sym] = 1;
			if (e.type == SDL_KEYUP)
				_keystate[e.key.keysym.sym] = 0;

		};
	};
};
#endif
