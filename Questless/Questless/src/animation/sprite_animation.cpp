//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/sprite_animation.hpp"

#include "game.hpp"
#include "utility/random.hpp"

#include <numeric>

using namespace media;

namespace ql {
	sprite_animation::sprite_animation(sprite_sheet sprite_sheet, std::vector<frame> frames, loop_type loop, start_time start_time)
	    : loop{loop}
	    , _sprite_sheet{std::move(sprite_sheet)}
	    , _frames{std::move(frames)}
	    , _accrued_time{start_time == start_time::random ? uniform(0.0, 1.0) * duration() : 0.0_s} {}

	sec sprite_animation::duration() const {
		return std::accumulate(
		    _frames.begin(), _frames.end(), 0.0_s, [](sec acc, frame const& frame) { return acc + frame.duration; });
	}

	void sprite_animation::reset(start_time start_time) {
		if (start_time == start_time::random) {
			// The next time update() is called, the animation will advance to a random point.
			_accrued_time = uniform(0.0, 1.0) * duration();
		}
		_frame_index = 0;
		_loops = 0;
		restart();
	}

	void sprite_animation::animation_subupdate(sec elapsed_time) {
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
					return;
				}
			} else if (_frame_index >= static_cast<int>(_frames.size())) {
				if (loop == loop_type::looping) {
					_frame_index = 0;
					++_loops;
				} else {
					stop();
					return;
				}
			}
		}

		// Update the origin based on the current frame.
		sf::Vector2i const texture_center = sf::Vector2i{_sprite_sheet.texture.getSize()} / 2;
		setOrigin(sf::Vector2f{texture_center + _frames[_frame_index].origin});
	}

	void sprite_animation::animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		sprite_animation::frame const& frame = _frames[_frame_index];
		sf::Sprite sprite{_sprite_sheet.texture, _sprite_sheet.get_cel_rect(frame.cel_coords)};
		sprite.setColor(color);
		target.draw(sprite, states);
	}
}
