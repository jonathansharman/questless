//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/Animation.h"
#include "Game.h"

using namespace units;

namespace questless
{
	Animation::Animation(std::vector<Frame> frames, Looping looping)
		: _frames{std::move(frames)}
		, _frame_index{0}
		, _accrued_time{GameSeconds::zero()}
		, _looping{looping}
		, _loops{0}
		, _in_reverse{false}
		, _time_scale{1.0}
		, _over{false}
	{}

	GameSeconds Animation::duration() const
	{
		auto total = GameSeconds::zero();
		for (Frame const& frame : _frames) {
			total += frame.duration;
		}
		return total;
	}

	void Animation::reset(RandomizeStartTime randomize_starting_time)
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

		_accrued_time += _time_scale * Game::frame_duration;
		while (_accrued_time > _frames[_frame_index].duration) {
			_accrued_time -= _frames[_frame_index].duration;
			if (_in_reverse) {
				--_frame_index;
			} else {
				++_frame_index;
			}

			if (_frame_index < 0) {
				if (_looping) {
					_frame_index = static_cast<int>(_frames.size() - 1);
					++_loops;
				} else {
					_over = true;
					break;
				}
			} else if (_frame_index >= static_cast<int>(_frames.size())) {
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
