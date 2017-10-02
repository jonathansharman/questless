//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>

#include "Animation.h"
#include "sdl/resources.h"
#include "units/SpriteSheetSpace.h"
#include "units/TextureSpace.h"
#include "units/GameSeconds.h"
#include "utility/Bounded.h"
#include "utility/reference.h"
#include "utility/TaggedType.h"

namespace questless
{
	struct Looping : TaggedType<bool> { using TaggedType::TaggedType; };
	struct RandomizeStartTime : TaggedType<bool> { using TaggedType::TaggedType; };

	//! A texture composed of a grid of animation cels.
	class SpriteSheet
	{
	public:
		sdl::TextureHandle const texture_handle;
		units::SpriteSheetSpace::Vector const cel_dimensions;

		//! @param texture_handle A texture handle to the sprite sheet texture.
		//! @param cel_dimensions The dimensions of this sprite sheet's cel grid.
		SpriteSheet(sdl::TextureHandle texture_handle, units::SpriteSheetSpace::Vector cel_dimensions)
			: texture_handle{texture_handle}, cel_dimensions{cel_dimensions}
		{}

		//! The dimensions of a single cel.
		units::ScreenSpace::Vector cel_size() const
		{
			if (_need_cel_size) {
				_cel_size = units::ScreenSpace::Vector
					{ sdl::texture_manager()[texture_handle].width() / cel_dimensions.x()
					, sdl::texture_manager()[texture_handle].height() / cel_dimensions.y()
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
		units::ScreenSpace::Vector mutable _cel_size;
	};

	//! A simple 2D animation.
	class SpriteAnimation : public Animation
	{
	public:
		struct Frame
		{
			units::GameSeconds duration;
			units::SpriteSheetSpace::Point coords; //!< The cel coordinates within the sprite sheet.
			units::TextureSpace::Vector origin; //!< The origin of the frame's texture relative to its center.

			Frame(units::GameSeconds duration, units::SpriteSheetSpace::Point coords, units::TextureSpace::Vector origin)
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
		SpriteAnimation(sptr<SpriteSheet> sprite_sheet, std::vector<Frame> frames, Looping looping);

		void draw(units::ScreenSpace::Point position) const final;

		void draw(units::GameSpace::Point position, Camera const& camera, units::colors::ColorFactor color_factor = units::colors::white_factor()) const final;

		//! The number of times the animation has looped.
		int loops() const { return _loops; }

		//! The total duration of the animation.
		units::GameSeconds duration() const;

		//! Moves to the start or a random time point in the animation, sets the loop counter to zero, and sets the over flag to false.
		//! @param randomize_start_time If true, resets the animation to a random time point.
		void reset(RandomizeStartTime randomize_start_time = RandomizeStartTime{false});
	private:
		sptr<SpriteSheet> _sprite_sheet;
		std::vector<Frame> _frames;
		int _frame_index;
		units::GameSeconds _accrued_time;
		int _loops;

		void animation_subupdate() final;
	};
}
