//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "sdl/resources.h"
#include "animation/Animation.h"
#include "animation/Camera.h"
#include "units/GamePoint.h"

namespace questless
{
	//! A set of associated animations with methods to control them.
	class AnimationSet
	{
	public:
		struct Handle
		{
			size_t index;
		};

		//! Constructs an animation collection pointer, which holds animations accessible by name.
		//! @param sprite_sheet_handle The handle of the sprite sheet texture in the texture manager.
		//! @param cel_columns The number of cels in one row of the sprite sheet texture.
		//! @param cel_rows The number of cels in one column of the sprite sheet texture.
		static auto make(sdl::TextureHandle sprite_sheet_handle, int cel_columns, int cel_rows)
		{
			return std::make_unique<AnimationSet>(sprite_sheet_handle, cel_columns, cel_rows);
		}

		//! Constructs an animation collection, which holds animations accessible by name.
		//! @param sprite_sheet_handle The handle of the sprite sheet texture in the texture manager.
		//! @param cel_columns The number of cels in one row of the sprite sheet texture.
		//! @param cel_rows The number of cels in one column of the sprite sheet texture.
		AnimationSet(sdl::TextureHandle sprite_sheet_handle, int cel_columns, int cel_rows);

		//! Adds an animation to the collection.
		//! @param animation The animation to be added.
		//! @return The handle to the new animation.
		Handle add(Animation animation);

		//! Starts the given animation from the beginning.
		//! @param animation_handle The handle of the animation to be started.
		//! @param randomize_start_time If true, resets the animation to a random time point.
		void start(Handle animation_handle, RandomizeStartTime randomize_start_time = RandomizeStartTime{false});

		//! Whether the current animation is paused.
		bool paused() const { return _paused; }

		//! Pauses the current animation.
		void pause() { _paused = true; }

		//! Resumes the current animation.
		void resume() { _paused = false; }

		//! Stops the current animation.
		void stop() { _current_animation = nullptr; }

		//! Updates the current animation.
		void update();

		//! Draws the current animation at the specified coordinates.
		//! @param origin The origin game point of the animation.
		void draw(units::GamePoint origin) const;

		//! Draws the current animation at the specified coordinates using the provided camera.
		//! @param origin The origin game point of the animation on the screen.
		//! @param camera A camera object.
		//! @param color An additional color multiplier, applied on top of the camera's and texture's color members.
		void draw(units::GamePoint origin, Camera const& camera, sdl::Color color = sdl::Color::white()) const;
	private:
		sdl::TextureHandle _sprite_sheet_handle;
		int _cel_width;
		int _cel_height;

		std::vector<Animation> _animations;
		Animation* _current_animation;

		bool _paused;
	};
}
