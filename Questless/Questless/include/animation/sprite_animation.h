//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>

#include "animation.h"
#include "sdl/resources.h"
#include "units/sprite_sheet_space.h"
#include "units/texture_space.h"
#include "utility/static_bounded.h"
#include "utility/reference.h"
#include "utility/tagged_type.h"

namespace ql
{
	struct looping : tagged_type<bool> { using tagged_type::tagged_type; };
	struct randomize_start_time : tagged_type<bool> { using tagged_type::tagged_type; };

	//! A texture composed of a grid of animation cels.
	class sprite_sheet
	{
	public:
		sdl::texture_handle const texture_handle;
		units::sprite_sheet_space::vector const cel_dimensions;

		//! @param texture_handle A texture handle to the sprite sheet texture.
		//! @param cel_dimensions The dimensions of this sprite sheet's cel grid.
		sprite_sheet(sdl::texture_handle texture_handle, units::sprite_sheet_space::vector cel_dimensions)
			: texture_handle{texture_handle}, cel_dimensions{cel_dimensions}
		{}

		//! The dimensions of a single cel.
		units::screen_space::vector cel_size() const
		{
			if (_need_cel_size) {
				_cel_size = units::screen_space::vector
					{ sdl::the_texture_manager()[texture_handle].width() / cel_dimensions.x()
					, sdl::the_texture_manager()[texture_handle].height() / cel_dimensions.y()
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
		units::screen_space::vector mutable _cel_size;
	};

	//! A simple 2D animation.
	class sprite_animation : public animation
	{
	public:
		struct frame
		{
			units::game_space::seconds duration;
			units::sprite_sheet_space::point coords; //!< The cel coordinates within the sprite sheet.
			units::texture_space::vector origin; //!< The origin of the frame's texture relative to its center.

			frame(units::game_space::seconds duration, units::sprite_sheet_space::point coords, units::texture_space::vector origin)
				: duration{duration}, coords{coords}, origin{origin}
			{}
		};

		//! If true, the animation starts over when it reaches the end. If false, it ends.
		bool looping;

		//! If true, the animation plays backwards.
		bool in_reverse;

		//! @param sprite_sheet The sprite sheet for this animation.
		//! @param frames The sequence of frames that compose the animation.
		//! @param looping Whether to loop the animation or play just once.
		sprite_animation(sptr<sprite_sheet> sprite_sheet, std::vector<frame> frames, ql::looping looping);

		void draw(units::screen_space::point position) const final;

		void draw(units::game_space::point position, camera const& camera, units::colors::color_factor color_factor = units::colors::white_factor()) const final;

		//! The number of times the animation has looped.
		int loops() const { return _loops; }

		//! The total duration of the animation.
		units::game_space::seconds duration() const;

		//! Moves to the start or a random time point in the animation, sets the loop counter to zero, and sets the over flag to false.
		//! @param randomize_start_time If true, resets the animation to a random time point.
		void reset(randomize_start_time randomize_start_time = randomize_start_time{false});
	private:
		sptr<sprite_sheet> _sprite_sheet;
		std::vector<frame> _frames;
		int _frame_index;
		units::game_space::seconds _accrued_time;
		int _loops;

		void animation_subupdate() final;
	};
}
