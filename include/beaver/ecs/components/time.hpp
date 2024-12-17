#pragma once

#include <array>
namespace beaver::component
{
	namespace timing
	{
		template<unsigned MSPF>
		struct normal_clock
		{
			unsigned _ms {0}, _s {0}, _m {0}, _h{0}, _spms {1000};		
			
			// tick the clock 
			void tick(float dt)
			{
				_ms += dt*MSPF;

				if (_ms >= _spms)
				{
					_ms = 0; _s++;
				};
				if (_s >= 60)
				{
					_s = 0; _m++;
				};
				if (_m >= 60)
				{
					_m = 0; _h++;
				};
			};

			// return hour, minute, second
			std::array<unsigned, 3> get_time(){return {_h, _m, _s};};
		};
		
		struct countdown
		{
			float _limit;
			float _elapsed {0};
			void reset(float limit)
			{
				_elapsed = 0;
				_limit = limit;
			};
			void update(float dt)
			{
				if (running()) _elapsed += dt;
			};
			float progress() const
			{
				return _elapsed / _limit;
			};
			bool running()
			{
				return _limit > _elapsed;
			};
		};

		struct stopwatch
		{
			float _elapsed;
			void reset()
			{
				_elapsed = 0;
			};
			void update(float dt)
			{
				_elapsed += dt;
			};
		};
	};
}

