/**
* @file    AnimationCollection.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the AnimationCollection class.
*/

#ifndef ANIMATION_COLLECTION_H
#define ANIMATION_COLLECTION_H

#include <string>
#include <map>
#include <memory>

#include "sdl-wrappers/resources.h"
#include "animation/Animation.h"
#include "animation/Camera.h"

namespace questless
{
	class AnimationCollection
	{
	public:
		using ptr = std::unique_ptr<AnimationCollection>;

		/// Constructs an animation collection pointer, which holds animations accessible by name.
		/// @param sprite_sheet_handle The handle of the sprite sheet texture in the texture manager.
		/// @param cel_columns The number of cels in one row of the sprite sheet texture.
		/// @param cel_rows The number of cels in one column of the sprite sheet texture.
		static ptr make(sdl::Handle<sdl::Texture> sprite_sheet_handle, int cel_columns, int cel_rows)
		{
			return std::make_unique<AnimationCollection>(sprite_sheet_handle, cel_columns, cel_rows);
		}

		/// Constructs an animation collection, which holds animations accessible by name.
		/// @param sprite_sheet_handle The handle of the sprite sheet texture in the texture manager.
		/// @param cel_columns The number of cels in one row of the sprite sheet texture.
		/// @param cel_rows The number of cels in one column of the sprite sheet texture.
		AnimationCollection(sdl::Handle<sdl::Texture> sprite_sheet_handle, int cel_columns, int cel_rows);

		/// Adds an animation to the collection.
		/// @param animation_name The name of the animation to be added.
		/// @param animation The animation to be added.
		void add(const std::string& animation_name, Animation::ptr animation);

		/// Starts the given animation from the beginning.
		/// @param animation_name The name of the animation to be started.
		/// @param randomize_starting_time If true, resets the animation to a random point.
		void start(const std::string& animation_name, bool randomize_starting_time = false);

		/// @return Whether the current animation is paused.
		bool paused() const { return _paused; }

		/// Pauses the current animation.
		void pause() { _paused = true; }

		/// Resumes the current animation.
		void resume() { _paused = false; }

		/// Stops the current animation.
		void stop() { _current_animation = nullptr; }

		/// Updates the current animation.
		void update();

		/// Draws the current animation at the specified coordinates.
		/// @param origin The origin game point of the animation.
		void draw(units::GamePoint origin) const;

		/// Draws the current animation at the specified coordinates using the provided camera.
		/// @param origin The origin game point of the animation on the screen.
		/// @param camera A camera object.
		void draw(units::GamePoint origin, const Camera& camera) const;
	private:
		sdl::Handle<sdl::Texture> _sprite_sheet_handle;
		int _cel_width;
		int _cel_height;

		std::map<std::string, Animation::ptr> _animations;
		Animation* _current_animation;

		bool _paused;
	};
}

#endif
