#ifndef BEAVER_TIME_H
#define BEAVER_TIME_H

#include <chrono>

namespace beaver
{
	struct FPS_tracker
	{
		int _FPS {60};
		std::chrono::steady_clock _clock;
		std::chrono::time_point<decltype(_clock)> _cur_frame, _prev_frame;
		void reset() {_cur_frame = _prev_frame = _clock.now();};
		int elapsed_time_ms() {return std::chrono::duration_cast<std::chrono::milliseconds>(_cur_frame - _prev_frame).count();};
		float dt_ratio() {return elapsed_time_ms() / (1000.f/_FPS);};
		bool new_frame_should_start(){_cur_frame = _clock.now(); return elapsed_time_ms() >= 1000.f/_FPS;};
		void end_frame (){_prev_frame = _cur_frame;};
	};

};

#endif
