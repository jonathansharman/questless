/**
* @file    Animation.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation of the Animation class.
*/

#include "animation/Animation.h"
#include "utility/utility.h"

using std::chrono::duration;

namespace questless
{
	Animation::Animation(std::vector<Frame> frames, bool looping)
		: _frames(frames)
		, _frame_index(0)
		, _accrued_time(double_seconds::zero())
		, _looping(looping)
		, _loops(0)
		, _in_reverse(false)
		, _time_scale(1.0)
		, _over(false)
	{}

	double_seconds Animation::duration() const
	{
		auto ret = double_seconds::zero();
		for (const Frame& frame : _frames) {
			ret += frame.duration;
		}
		return ret;
	}

	void Animation::reset(bool randomize_starting_time)
	{
		if (randomize_starting_time) {
			// The next time update() is called, the animation will advance to a random point.
			_accrued_time = uniform(0.0, 1.0) * duration();
		}
		_frame_index = 0;
		_loops = 0;
		_over = false;
	}

	void Animation::update()
	{
		if (_over) {
			return;
		}

		_accrued_time += _time_scale * frame_duration;
		while (_accrued_time > _frames[_frame_index].duration) {
			_accrued_time -= _frames[_frame_index].duration;
			if (_in_reverse) {
				--_frame_index;
			} else {
				++_frame_index;
			}

			if (_frame_index < 0) {
				if (_looping) {
					_frame_index = _frames.size() - 1;
					++_loops;
				} else {
					_over = true;
					break;
				}
			} else if (static_cast<size_t>(_frame_index) >= _frames.size()) {
				if (_looping) {
					_frame_index = 0;
					++_loops;
				} else {
					_over = true;
					break;
				}
			}
		}
	}
}