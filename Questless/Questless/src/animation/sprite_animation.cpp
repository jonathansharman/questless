//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sprite_animation.hpp"

#include "utility/random.hpp"

#include <numeric>

namespace ql {
	sprite_animation::sprite_animation(sprite_sheet sprite_sheet, std::vector<frame> frames, loop_type loop, start_time start_time)
		: loop{loop}
		, _sprite_sheet{std::move(sprite_sheet)}
		, _frames{std::move(frames)}
		, _accrued_time{start_time == start_time::random ? uniform(0.0f, 1.0f) * duration() : 0.0_s} {}

	auto sprite_animation::duration() const -> sec {
		return std::accumulate(
			_frames.begin(), _frames.end(), 0.0_s, [](sec acc, frame const& frame) { return acc + frame.duration; });
	}

	auto sprite_animation::reset(start_time start_time) -> void {
		if (start_time == start_time::random) {
			// The next time update() is called, the animation will advance to a random point.
			_accrued_time = uniform(0.0f, 1.0f) * duration();
		}
		_frame_index = 0;
		_loops = 0;
		restart();
	}

	auto sprite_animation::animation_subupdate(sec elapsed_time) -> void {
		_accrued_time += elapsed_time;
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
					stop();
					break;
				}
			} else if (_frame_index >= static_cast<int>(_frames.size())) {
				if (loop == loop_type::looping) {
					_frame_index = 0;
					++_loops;
				} else {
					stop();
					break;
				}
			}
		}

		// Update the origin based on the current frame.
		setOrigin(sf::Vector2f{_frames[_frame_index].origin});
	}

	auto sprite_animation::animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		sprite_animation::frame const& frame = _frames[_frame_index];
		sf::Sprite sprite{_sprite_sheet.texture, _sprite_sheet.get_cel_rect(frame.cel_coords)};
		sprite.setColor(color);
		sprite.setOrigin(getOrigin());
		target.draw(sprite, states);
	}
}
