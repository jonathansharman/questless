//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation.hpp"
#include "sprite_sheet.hpp"

#include "ui/view_space.hpp"
#include "utility/reference.hpp"

#include "cancel/quantity.hpp"

#include <vector>

namespace ql {
	//! A simple 2D animation.
	struct sprite_animation : animation {
		enum class start_time { zero, random };

		struct frame {
			//! How long to display this frame, at normal time scale.
			sec duration;

			//! The cel coordinates within the sprite sheet.
			sf::Vector2i cel_coords;

			//! The origin of this frame relative to the center of its texture.
			sf::Vector2i origin;
		};

		enum class loop_type { once, looping } loop;

		enum class direction_type { forward, reverse } direction = direction_type::forward;

		//! Color applied to the sprite sheet before drawing.
		sf::Color color = sf::Color::White;

		sprite_animation(sprite_sheet sprite_sheet, std::vector<frame> frames, loop_type loop, start_time start_time = start_time::zero);

		//! The number of times the animation has looped.
		int loops() const {
			return _loops;
		}

		//! The total duration of the animation.
		sec duration() const;

		//! Moves to the start or a random time point in the animation, sets the loop counter to zero, and sets the over
		//! flag to false.
		void reset(start_time start_time = start_time::zero);

	private:
		sprite_sheet _sprite_sheet;
		std::vector<frame> _frames;
		int _frame_index = 0;
		sec _accrued_time;
		int _loops = 0;

		void animation_subupdate(sec elapsed_time) final;

		void animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;
	};
}
