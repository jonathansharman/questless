/**
* @file    AnimationCollection.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation of the AnimationCollection class.
*/

#include "animation/AnimationCollection.h"

using namespace sdl;
using namespace units;

/// @todo Make AnimationCollection a Renderable and cache the sprite sheet?

namespace questless
{
	AnimationCollection::AnimationCollection(sdl::Handle<Texture> sprite_sheet_handle, int cel_columns, int cel_rows)
		: _sprite_sheet_handle{std::move(sprite_sheet_handle)}, _current_animation{nullptr}, _paused{false}
	{
		_cel_width = texture_manager()[_sprite_sheet_handle].width() / cel_columns;
		_cel_height = texture_manager()[_sprite_sheet_handle].height() / cel_rows;
	}

	void AnimationCollection::add(const std::string& animation_name, Animation::ptr animation)
	{
		_animations[animation_name] = std::move(animation);
	}
	
	void AnimationCollection::start(const std::string& animation_name, bool randomize_starting_time)
	{
		_current_animation = _animations[animation_name].get();
		if (_current_animation != nullptr) {
			_current_animation->reset(randomize_starting_time);
		}
	}

	void AnimationCollection::update()
	{
		if (!_paused && _current_animation != nullptr) {
			_current_animation->update();
		}
	}

	void AnimationCollection::draw(GamePoint origin) const
	{
		if (_current_animation != nullptr) {
			const Animation::Frame& frame = _current_animation->current_frame();
			ScreenRect dst_rect{lround(origin.x - frame.origin.x), lround(origin.y - frame.origin.y), _cel_width, _cel_height};
			TextureRect src_rect{_cel_width * frame.coords.x, _cel_height * frame.coords.y, _cel_width, _cel_height};
			texture_manager()[_sprite_sheet_handle].draw(dst_rect, src_rect);
		}
	}

	void AnimationCollection::draw(GamePoint origin, const Camera& camera) const
	{
		if (_current_animation != nullptr) {
			const Animation::Frame& frame = _current_animation->current_frame();
			camera.draw
				( texture_manager()[_sprite_sheet_handle]
				, GamePoint{origin - GameVector{static_cast<double>(frame.origin.x), static_cast<double>(-frame.origin.y)}} /// @todo Uncouth point casting here.
				, Origin{boost::none}
				, Color::white()
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
