//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/SpriteAnimation.h"
#include "Game.h"

using namespace sdl;
using namespace units;

namespace questless
{
	SpriteAnimation::SpriteAnimation(sptr<SpriteSheet> sprite_sheet, std::vector<Frame> frames, Looping looping)
		: looping{looping}
		, in_reverse{false}
		, _sprite_sheet{std::move(sprite_sheet)}
		, _frames{std::move(frames)}
		, _frame_index{0}
		, _accrued_time{GameSeconds::zero()}
		, _loops{0}
	{}

	void SpriteAnimation::draw(units::ScreenSpace::Point position) const
	{
		//! @todo What is the default center in this case, and what should it be?
		SpriteAnimation::Frame const& frame = _frames[_frame_index];
		ScreenSpace::Box dst_rect
			{ ScreenSpace::Point{position.x() - frame.origin.u(), position.y() - frame.origin.v()} //! @todo Uncouth point casting here.
			, ScreenSpace::Vector{_sprite_sheet->cel_width(), _sprite_sheet->cel_height()}
			};
		TextureSpace::Box src_rect
			{ TextureSpace::Point{_sprite_sheet->cel_width() * frame.coords.x(), _sprite_sheet->cel_height() * frame.coords.y()}
			, TextureSpace::Vector{_sprite_sheet->cel_width(), _sprite_sheet->cel_height()}
			};
		texture_manager()[_sprite_sheet->texture_handle].draw(dst_rect, colors::white_factor(), src_rect);
	}

	void SpriteAnimation::draw(units::GameSpace::Point position, Camera const& camera, colors::ColorFactor color_factor) const
	{
		SpriteAnimation::Frame const& frame = _frames[_frame_index];
		camera.draw
			( texture_manager()[_sprite_sheet->texture_handle]
			, GameSpace::Point{position}
			, Origin{frame.origin}
			, color_factor
			, HScale{1.0}
			, VScale{1.0}
			, GameSpace::Radians{0.0}
			, SrcRect{TextureSpace::Box
				{ TextureSpace::Point{_sprite_sheet->cel_width() * frame.coords.x(), _sprite_sheet->cel_height() * frame.coords.y()}
				, TextureSpace::Vector{_sprite_sheet->cel_width(), _sprite_sheet->cel_height()}
				}}
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
		_accrued_time += elapsed_time();
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
