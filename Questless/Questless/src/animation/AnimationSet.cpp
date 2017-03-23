/**
* @file    AnimationSet.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation of the AnimationSet class.
*/

#include "animation/AnimationSet.h"

#include "units/ScreenRect.h"
#include "units/GameVector.h"

using namespace sdl;
using namespace units;

namespace questless
{
	AnimationSet::AnimationSet(sdl::TextureHandle sprite_sheet_handle, int cel_columns, int cel_rows)
		: _sprite_sheet_handle{std::move(sprite_sheet_handle)}, _current_animation{nullptr}, _paused{false}
	{
		_cel_width = texture_manager()[_sprite_sheet_handle].width() / cel_columns;
		_cel_height = texture_manager()[_sprite_sheet_handle].height() / cel_rows;
	}

	AnimationSet::Handle AnimationSet::add(Animation animation)
	{
		_animations.push_back(std::move(animation));
		return Handle{_animations.size() - 1};
	}
	
	void AnimationSet::start(Handle animation_handle, RandomizeStartTime randomize_start_time)
	{
		_current_animation = &_animations[animation_handle.index];
		_current_animation->reset(randomize_start_time);
	}

	void AnimationSet::update()
	{
		if (!_paused && _current_animation != nullptr) {
			_current_animation->update();
		}
	}

	void AnimationSet::draw(GamePoint origin) const
	{
		if (_current_animation != nullptr) {
			Animation::Frame const& frame = _current_animation->current_frame();
			ScreenRect dst_rect{lround(origin.x - frame.origin.x), lround(origin.y - frame.origin.y), _cel_width, _cel_height};
			TextureRect src_rect{_cel_width * frame.coords.x, _cel_height * frame.coords.y, _cel_width, _cel_height};
			texture_manager()[_sprite_sheet_handle].draw(dst_rect, src_rect);
		}
	}

	void AnimationSet::draw(GamePoint origin, Camera const& camera, Color color) const
	{
		if (_current_animation != nullptr) {
			Animation::Frame const& frame = _current_animation->current_frame();
			camera.draw
				( texture_manager()[_sprite_sheet_handle]
				, GamePoint{origin - GameVector{static_cast<double>(frame.origin.x), static_cast<double>(-frame.origin.y)}} /// @todo Uncouth point casting here.
				, Origin{std::nullopt}
				, color
				, HScale{1.0}
				, VScale{1.0}
				, GameRadians{0.0}
				, HFlip{false}
				, VFlip{false}
				, SrcRect{TextureRect{_cel_width * frame.coords.x, _cel_height * frame.coords.y, _cel_width, _cel_height}}
				);
		}
	}
}
