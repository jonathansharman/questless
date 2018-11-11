//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation.hpp"
#include "utility/reference.hpp"

#include "cancel/quantity.hpp"

#include "sdl/resources.hpp"
#include "units/sprite_sheet_space.hpp"
#include "units/texture_space.hpp"

#include <vector>

namespace ql {
	//! A texture composed of a grid of animation cels.
	class sprite_sheet {
	public:
		sdl::texture_handle const texture_handle;
		units::sprite_sheet_space::vector const cel_dimensions;

		//! @param texture_handle A texture handle to the sprite sheet texture.
		//! @param cel_dimensions The dimensions of this sprite sheet's cel grid.
		sprite_sheet(sdl::texture_handle texture_handle, units::sprite_sheet_space::vector cel_dimensions)
			: texture_handle{texture_handle}, cel_dimensions{cel_dimensions}
		{}

		//! The dimensions of a single cel.
		spaces::window::vector cel_size() const {
			if (_need_cel_size) {
				_cel_size = spaces::window::vector {
					sdl::the_texture_manager()[texture_handle].width() / cel_dimensions.x(),
					sdl::the_texture_manager()[texture_handle].height() / cel_dimensions.y(),
				};
				_need_cel_size = false;
			}
			return _cel_size;
		}

		//! The width of a single cel.
		int cel_width() const { return cel_size().x(); }

		//! The width of a single cel.
		int cel_height() const { return cel_size().y(); }
	private:
		bool mutable _need_cel_size = true;
		spaces::window::vector mutable _cel_size;
	};

	//! A simple 2D animation.
	class sprite_animation : public animation {
	public:
		enum class start_time { zero, random };

		struct frame {
			//! How long to display this frame, at normal time scale.
			sec duration;

			//! The cel coordinates within the sprite sheet.
			units::sprite_sheet_space::point coords;

			//! The origin of the frame's texture relative to its center.
			units::texture_space::vector origin;
		};

		enum class loop_type { once, looping } loop;

		enum class direction_type { forward, reverse } direction = direction_type::forward;

		sprite_animation
			( sptr<sprite_sheet> sprite_sheet
			, std::vector<frame> frames
			, loop_type loop
			, start_time start_time = start_time::zero
			);

		void draw(spaces::window::point position) const final;

		void draw
			( units::world_space::point position
			, camera const& camera
			, spaces::colors::color_vector color_vector = spaces::colors::white_vector()
			) const final;

		//! The number of times the animation has looped.
		int loops() const { return _loops; }

		//! The total duration of the animation.
		sec duration() const;

		//! Moves to the start or a random time point in the animation, sets the loop counter to zero, and sets the over flag to false.
		void reset(start_time start_time = start_time::zero);
	private:
		sptr<sprite_sheet> _sprite_sheet;
		std::vector<frame> _frames;
		int _frame_index = 0;
		sec _accrued_time;
		int _loops = 0;

		void animation_subupdate() final;
	};
}
