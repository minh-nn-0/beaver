#ifndef BEAVER_PARTICLES_H
#define BEAVER_PARTICLES_H

#include <mmath/core.hpp>
#include <Beaver/entity.hpp>
#include <sdlwrapper/sdlwrapper.hpp>

namespace beaver
{
	namespace particles
	{
		struct manager
		{
			void emit(int number);
			void update(float dt);
			void draw();
		};
	};
};

#endif
