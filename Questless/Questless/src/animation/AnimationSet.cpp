//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/AnimationSet.h"

#include "units/ScreenRect.h"
#include "units/GameVector.h"

using namespace sdl;
using namespace units;

namespace questless
{
	AnimationSet::Handle AnimationSet::add(Animation animation)
	{
		_animations.push_back(std::move(animation));
		return Handle{_animations.size() - 1};
	}
	
	void AnimationSet::start(Handle animation_handle, RandomizeStartTime randomize_start_time)
	{
		_current_animation_idx = animation_handle.index;
		current_animation().reset(randomize_start_time);
	}

	void AnimationSet::update()
	{
		if (!_paused && _current_animation_idx) {
			current_animation().update();
		}
	}

	void AnimationSet::draw(GamePoint origin) const
	{
		if (_current_animation_idx) {
			Texture const& sprite_sheet = texture_manager()[_sprite_sheet_handle];
			int const cel_width = sprite_sheet.width() / _cel_columns;
			int const cel_height = sprite_sheet.height() / _cel_rows;
			Animation::Frame const& frame = current_animation().current_frame();
			ScreenRect dst_rect{lround(origin.x - frame.origin.x), lround(origin.y - frame.origin.y), cel_width, cel_height};
			TextureRect src_rect{cel_width * frame.coords.x, cel_height * frame.coords.y, cel_width, cel_height};
			sprite_sheet.draw(dst_rect, src_rect);
		}
	}

	void AnimationSet::draw(GamePoint origin, Camera const& camera, Color color) const
	{
		if (_current_animation_idx) {
			Texture const& sprite_sheet = texture_manager()[_sprite_sheet_handle];
			int const cel_width = sprite_sheet.width() / _cel_columns;
			int const cel_height = sprite_sheet.height() / _cel_rows;
			Animation::Frame const& frame = current_animation().current_frame();
			camera.draw
				( texture_manager()[_sprite_sheet_handle]
				, GamePoint{origin - GameVector{static_cast<double>(frame.origin.x), static_cast<double>(-frame.origin.y)}} //! @todo Uncouth point casting here.
				, Origin{std::nullopt}
				, color
				, HScale{1.0}
				, VScale{1.0}
				, GameRadians{0.0}
				, HFlip{false}
				, VFlip{false}
				, SrcRect{TextureRect{cel_width * frame.coords.x, cel_height * frame.coords.y, cel_width, cel_height}}
				);
		}
	}
}
