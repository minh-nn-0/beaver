#ifndef M_INPUT_H
#define M_INPUT_H

#include <unordered_map>
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

	const std::unordered_map<std::string, unsigned> KEYMAP {
	    {"A", SDLK_a},
		{"B", SDLK_b},
		{"C", SDLK_c},
		{"D", SDLK_d},
		{"E", SDLK_e},
		{"F", SDLK_f},
		{"G", SDLK_g},
		{"H", SDLK_h},
		{"I", SDLK_i},
		{"J", SDLK_j},
		{"K", SDLK_k},
		{"L", SDLK_l},
		{"M", SDLK_m},
		{"N", SDLK_n},
		{"O", SDLK_o},
		{"P", SDLK_p},
		{"Q", SDLK_q},
		{"R", SDLK_r},
		{"S", SDLK_s},
		{"T", SDLK_t},
		{"U", SDLK_u},
		{"V", SDLK_v},
		{"W", SDLK_w},
		{"X", SDLK_x},
		{"Y", SDLK_y},
		{"Z", SDLK_z},
		{"0", SDLK_0},
		{"1", SDLK_1},
		{"2", SDLK_2},
		{"3", SDLK_3},
		{"4", SDLK_4},
		{"5", SDLK_5},
		{"6", SDLK_6},
		{"7", SDLK_7},
		{"8", SDLK_8},
		{"9", SDLK_9},
		{"SPACE", SDLK_SPACE},
		{"ENTER", SDLK_RETURN},
		{"ESCAPE", SDLK_ESCAPE},
		{"LEFT", SDLK_LEFT},
		{"RIGHT", SDLK_RIGHT},
		{"UP", SDLK_UP},
		{"DOWN", SDLK_DOWN},
		{"SHIFT", SDLK_LSHIFT},
		{"CTRL", SDLK_LCTRL},
		{"ALT", SDLK_LALT},
		{"TAB", SDLK_TAB},
		{"BACKSPACE", SDLK_BACKSPACE},
		{"DELETE", SDLK_DELETE},
		{"CAPSLOCK", SDLK_CAPSLOCK},
		{"F1", SDLK_F1},
		{"F2", SDLK_F2},
		{"F3", SDLK_F3},
		{"F4", SDLK_F4},
		{"F5", SDLK_F5},
		{"F6", SDLK_F6},
		{"F7", SDLK_F7},
		{"F8", SDLK_F8},
		{"F9", SDLK_F9},
		{"F10", SDLK_F10},
		{"F11", SDLK_F11},
		{"F12", SDLK_F12},
		{"COMMA", SDLK_COMMA},
		{"PERIOD", SDLK_PERIOD},
		{"SLASH", SDLK_SLASH},
		{"SEMICOLON", SDLK_SEMICOLON},
		{"APOSTROPHE", SDLK_QUOTE},
		{"LEFTBRACKET", SDLK_LEFTBRACKET},
		{"RIGHTBRACKET", SDLK_RIGHTBRACKET},
		{"BACKSLASH", SDLK_BACKSLASH},
		{"MINUS", SDLK_MINUS},
		{"EQUALS", SDLK_EQUALS},
		{"GRAVE", SDLK_BACKQUOTE}
	};
};
#endif
