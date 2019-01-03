//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/sprite_animation.hpp"

#include "animation/camera.hpp"
#include "game.hpp"
#include "utility/random.hpp"

#include <numeric>

using namespace sdl;
using namespace sdl::spaces::sprite_sheet::literals;
using namespace sdl::spaces::view::literals;

using namespace vecx::literals;

namespace ql {
	sprite_animation::sprite_animation
		( sptr<sprite_sheet> sprite_sheet
		, std::vector<frame> frames
		, loop_type loop
		, start_time start_time
		)
		: loop{loop}
		, _sprite_sheet{std::move(sprite_sheet)}
		, _frames{std::move(frames)}
		, _accrued_time{start_time == start_time::random ? uniform(0.0, 1.0) * duration() : 0.0_s}
	{}

	void sprite_animation::draw(spaces::window::point position) const {
		//! @todo What is the default center in this case, and what should it be?
		sprite_animation::frame const& frame = _frames[_frame_index];
		//spaces::window::box src_rect
		//	{ spaces::window::point{_sprite_sheet->cel_width() * frame.coords.x(), _sprite_sheet->cel_height() * frame.coords.y()}
		//	, spaces::window::vector{_sprite_sheet->cel_width(), _sprite_sheet->cel_height()}
		//	};
		//the_texture_manager()[_sprite_sheet->texture_handle].draw
		//	( spaces::window::point{position.x() - frame.origin.u(), position.y() + frame.origin.v()} //! @todo Uncouth point casting here.
		//	, h_align::center
		//	, v_align::middle
		//	, colors::white_vector()
		//	, src_rect
		//	);
		the_texture_manager()[_sprite_sheet->texture_handle].draw_transformed
			( position
			, frame.origin
			, spaces::colors::white()
			, {1.0_view_length, 1.0_view_length}
			, 0.0_rad
			, spaces::window::box
				{ spaces::window::point{_sprite_sheet->cel_width() * frame.coords[0] / 1_cel, _sprite_sheet->cel_height() * frame.coords[1] / 1_cel}
				, spaces::window::vector{_sprite_sheet->cel_width(), _sprite_sheet->cel_height()}
				}
			);
	}

	void sprite_animation::draw(world::point position, camera const& camera, spaces::colors::color color_factor) const {
		sprite_animation::frame const& frame = _frames[_frame_index];
		camera.draw
			( the_texture_manager()[_sprite_sheet->texture_handle]
			, world::point{position}
			, frame.origin
			, color_factor
			, {1.0_view_length, 1.0_view_length}
			, 0.0_rad
			, spaces::window::box
				{ spaces::window::point{_sprite_sheet->cel_width() * frame.coords[0] / 1_cel, _sprite_sheet->cel_height() * frame.coords[1] / 1_cel}
				, spaces::window::vector{_sprite_sheet->cel_width(), _sprite_sheet->cel_height()}
				}
			);
	}

	sec sprite_animation::duration() const {
		return std::accumulate(_frames.begin(), _frames.end(), 0.0_s, [](sec acc, frame const& frame) {
			return acc + frame.duration;
		});
	}

	void sprite_animation::reset(start_time start_time) {
		if (start_time == start_time::random) {
			// The next time update() is called, the animation will advance to a random point.
			_accrued_time = uniform(0.0, 1.0) * duration();
		}
		_frame_index = 0;
		_loops = 0;
		_over = false;
	}

	void sprite_animation::animation_subupdate() {
		_accrued_time += elapsed_time();
		while (_accrued_time > _frames[_frame_index].duration) {
			_accrued_time -= _frames[_frame_index].duration;
			if (direction == direction_type::reverse) {
				--_frame_index;
			} else {
				++_frame_index;
			}

			if (_frame_index < 0) {
				if (loop == loop_type::looping) {
					_frame_index = static_cast<int>(_frames.size() - 1);
					++_loops;
				} else {
					_over = true;
					break;
				}
			} else if (_frame_index >= static_cast<int>(_frames.size())) {
				if (loop == loop_type::looping) {
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
