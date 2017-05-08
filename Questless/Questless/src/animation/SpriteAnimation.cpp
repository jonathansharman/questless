//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/SpriteAnimation.h"
#include "Game.h"

using namespace units;
using namespace sdl;

namespace questless
{
	SpriteAnimation::SpriteAnimation(sptr<SpriteSheet> sprite_sheet, std::vector<Frame> frames, Looping looping)
		: looping{looping}
		, in_reverse{false}
		, time_scale{1.0}
		, _sprite_sheet{std::move(sprite_sheet)}
		, _frames{std::move(frames)}
		, _frame_index{0}
		, _accrued_time{GameSeconds::zero()}
		, _loops{0}
		, _over{false}
	{}

	void SpriteAnimation::draw(units::ScreenPoint position) const
	{
		SpriteAnimation::Frame const& frame = _frames[_frame_index];
		ScreenRect dst_rect{position.x - frame.origin.x, position.y - frame.origin.y, _sprite_sheet->cel_width(), _sprite_sheet->cel_height()};
		TextureRect src_rect
			{ _sprite_sheet->cel_width() * frame.coords.x
			, _sprite_sheet->cel_height() * frame.coords.y
			, _sprite_sheet->cel_width()
			, _sprite_sheet->cel_height()
			};
		texture_manager()[_sprite_sheet->texture_handle].draw(dst_rect, src_rect);
	}

	void SpriteAnimation::draw(units::GamePoint position, Camera const& camera, sdl::Color color) const
	{
		SpriteAnimation::Frame const& frame = _frames[_frame_index];
		camera.draw
			( texture_manager()[_sprite_sheet->texture_handle]
			, GamePoint{position - GameVector{static_cast<double>(frame.origin.x), static_cast<double>(-frame.origin.y)}} //! @todo Uncouth point casting here.
			, Origin{std::nullopt}
			, color
			, HScale{1.0}
			, VScale{1.0}
			, GameRadians{0.0}
			, HFlip{false}
			, VFlip{false}
			, SrcRect{TextureRect{_sprite_sheet->cel_width() * frame.coords.x, _sprite_sheet->cel_height() * frame.coords.y, _sprite_sheet->cel_width(), _sprite_sheet->cel_height()}}
			);
	}

	GameSeconds SpriteAnimation::duration() const
	{
		auto total = GameSeconds::zero();
		for (Frame const& frame : _frames) {
			total += frame.duration;
		}
		return total;
	}

	void SpriteAnimation::reset(RandomizeStartTime randomize_starting_time)
	{
		if (randomize_starting_time) {
			// The next time update() is called, the animation will advance to a random point.
			_accrued_time = uniform(0.0, 1.0) * duration();
		}
		_frame_index = 0;
		_loops = 0;
		_over = false;
	}

	void SpriteAnimation::animation_subupdate()
	{
		if (!_over) {
			_accrued_time += time_scale.get() * Game::frame_duration;
			while (_accrued_time > _frames[_frame_index].duration) {
				_accrued_time -= _frames[_frame_index].duration;
				if (in_reverse) {
					--_frame_index;
				} else {
					++_frame_index;
				}

				if (_frame_index < 0) {
					if (looping) {
						_frame_index = static_cast<int>(_frames.size() - 1);
						++_loops;
					} else {
						_over = true;
						break;
					}
				} else if (_frame_index >= static_cast<int>(_frames.size())) {
					if (looping) {
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
}
