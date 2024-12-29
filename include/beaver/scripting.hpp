#ifndef BEAVER_SCRIPTING_H
#define BEAVER_SCRIPTING_H

#include <sol/sol.hpp>
#include <mmath/core.hpp>
#include <beaver/sdlgame.hpp>
#include <tiledwrapper/tiledwrapper.hpp>

// TODO: For some reasons these bind functions if use all at one (for example use the beaver::bind_ecs_core_component ; 
// using a few is fine, tried multiple combinations and doesn't look like any is the suspect) will
// crash the program with a lot of weird linker errors on Windows, only in Debug Mode. And we don't fking know why 
// 	Tried 
// 	- increased stack size
//	- inline all functions
//	- remove unused arguments

namespace beaver
{
};

#endif
