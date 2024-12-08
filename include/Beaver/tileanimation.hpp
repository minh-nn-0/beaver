#ifndef M_ANIMATION_H
#define M_ANIMATION_H

#include <vector>

// ----------------------------------
// ANIMATION
// ----------------------------------
namespace beaver
{
	// run at 60fps -> 17ms a frame
	struct tile_animation
	{
		struct frame
		{
			int _duration;
			int _id;
		};
		using framevec = std::vector<frame>;

		framevec _frames;
		framevec::iterator _current_frame {_frames.begin()};
		float _timer {0};
		bool _repeat {true};
		bool _playing {true};
		bool _frame_changed {false};
		
		void new_frames(const framevec& frames)
		{
			_frames = frames;
			reset();
		};

		void set_frames(const framevec& frames)
		{
			_frames = frames;
			reset();
		};
		bool frame_is_end() {return _current_frame == _frames.end() -1;};
		bool frame_is_start() {return _current_frame == _frames.begin();};
		void play() {_playing = true;};
		void reset() {_current_frame = _frames.begin(); _timer = 0;};
		int current_id() {return _current_frame->_id;};
		void update(float dtr)
		{
			_frame_changed = false;
			if (_playing)
			{
				_timer += dtr * 17;
				if (_timer >= _current_frame->_duration) 
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
