#ifndef BEAVER_TIME_H
#define BEAVER_TIME_H

#include <chrono>

namespace beaver
{
	struct FPS_tracker
	{
		int _FPS {60};
		std::chrono::steady_clock _cl;
		std::chrono::time_point<decltype(_cl)> _cur_frame, _prev_frame;
		void reset() {_cur_frame = _prev_frame = _cl.now();};
		int elapsed_time_ms() {return std::chrono::duration_cast<std::chrono::milliseconds>(_cur_frame - _prev_frame).count();};
		float dt_ratio() {return elapsed_time_ms() / (1000.f/_FPS);};
		bool new_frame_should_start(){_cur_frame = _cl.now(); return elapsed_time_ms() >= 1000.f/_FPS;};
		void end_frame (){_prev_frame = _cur_frame;};
	};

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
	
	namespace timer
	{
		// use milliseconds
		struct countdown
		{
			unsigned _limit;
			unsigned _elapsed {0};
			void reset(unsigned limit)
			{
				_elapsed = 0;
				_limit = limit;
			};
			void update(float dtr)
			{
				_elapsed += dtr/60;
			};
			bool running()
			{
				return _limit > _elapsed;
			};
		};
	};
};

#endif
