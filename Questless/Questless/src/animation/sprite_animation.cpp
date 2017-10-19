//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/sprite_animation.hpp"
#include "game.hpp"

using namespace sdl;
using namespace units;

namespace ql
{
	sprite_animation::sprite_animation(sptr<sprite_sheet> sprite_sheet, std::vector<frame> frames, ql::looping looping)
		: looping{looping}
		, in_reverse{false}
		, _sprite_sheet{std::move(sprite_sheet)}
		, _frames{std::move(frames)}
		, _frame_index{0}
		, _accrued_time{game_space::seconds::zero()}
		, _loops{0}
	{}

	void sprite_animation::draw(units::screen_space::point position) const
	{
		//! @todo What is the default center in this case, and what should it be?
		sprite_animation::frame const& frame = _frames[_frame_index];
		//screen_space::box src_rect
		//	{ screen_space::point{_sprite_sheet->cel_width() * frame.coords.x(), _sprite_sheet->cel_height() * frame.coords.y()}
		//	, screen_space::vector{_sprite_sheet->cel_width(), _sprite_sheet->cel_height()}
		//	};
		//the_texture_manager()[_sprite_sheet->texture_handle].draw
		//	( screen_space::point{position.x() - frame.origin.u(), position.y() + frame.origin.v()} //! @todo Uncouth point casting here.
		//	, h_align::center
		//	, v_align::middle
		//	, colors::white_factor()
		//	, src_rect
		//	);
		the_texture_manager()[_sprite_sheet->texture_handle].draw_transformed
			( position
			, frame.origin
			, colors::white_factor()
			, 1.0f
			, 1.0f
			, game_space::radians{0.0}
			, src_rect{texture_space::box
				{ texture_space::point{_sprite_sheet->cel_width() * frame.coords.x(), _sprite_sheet->cel_height() * frame.coords.y()}
				, texture_space::vector{_sprite_sheet->cel_width(), _sprite_sheet->cel_height()}
				}}
			);
	}

	void sprite_animation::draw(units::game_space::point position, camera const& camera, colors::color_factor color_factor) const
	{
		sprite_animation::frame const& frame = _frames[_frame_index];
		camera.draw
			( the_texture_manager()[_sprite_sheet->texture_handle]
			, game_space::point{position}
			, origin{frame.origin}
			, color_factor
			, h_scale{1.0}
			, v_scale{1.0}
			, game_space::radians{0.0}
			, src_rect{texture_space::box
				{ texture_space::point{_sprite_sheet->cel_width() * frame.coords.x(), _sprite_sheet->cel_height() * frame.coords.y()}
				, texture_space::vector{_sprite_sheet->cel_width(), _sprite_sheet->cel_height()}
				}}
			);
	}

	game_space::seconds sprite_animation::duration() const
	{
		auto total = game_space::seconds::zero();
		for (frame const& frame : _frames) {
			total += frame.duration;
		}
		return total;
	}

	void sprite_animation::reset(randomize_start_time randomize_starting_time)
	{
		if (randomize_starting_time) {
			// The next time update() is called, the animation will advance to a random point.
			_accrued_time = uniform(0.0, 1.0) * duration();
		}
		_frame_index = 0;
		_loops = 0;
		_over = false;
	}

	void sprite_animation::animation_subupdate()
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
