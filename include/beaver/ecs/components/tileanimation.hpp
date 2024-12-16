#ifndef M_ANIMATION_H
#define M_ANIMATION_H

#include <vector>

// ----------------------------------
// ANIMATION
// ----------------------------------
namespace beaver::component
{
	struct tile_animation
	{
		struct frame
		{
			int _id;
			int _duration;
		};
		using framevec = std::vector<frame>;

		framevec _frames;
		std::size_t _current_frame {0};
		int _framewidth, _frameheight;
		float _timer {0};
		bool _repeat {true};
		bool _playing {true};
		bool _frame_changed {false};
		
		void new_frames(const framevec& frames)
		{
			_frames = frames;
			reset();
		};

		bool frame_is_end() {return _current_frame == _frames.size() -1;};
		bool frame_is_start() {return _current_frame == 0;};
		void play() {_playing = true;};
		void reset() {_current_frame = 0; _timer = 0; _playing = true;};
		frame& current_frame() {return _frames.at(_current_frame);};
		void update(float dt)
		{
			_frame_changed = false;
			if (_playing)
			{
				_timer += dt * 1000;
				if (_timer >= _frames.at(_current_frame)._duration) 
				{
					_timer = 0;
					// when change frames, _frames is different so _current_frame never == _frames.end() -1
					// so it increments infinitely
					// -> use constructor. don't assign to _frames directly
					if (frame_is_end())
					{
						if (!_repeat) _playing = false;
						else reset();
					}
					else _current_frame++;
					_frame_changed = true;
				};
			};
		};
	};
};

#endif
