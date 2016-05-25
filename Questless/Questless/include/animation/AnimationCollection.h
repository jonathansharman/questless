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

#include <memory>
using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
#include <map>
using std::map;

#include "sdl-wrappers/Texture.h"
using sdl::Texture;
#include "animation/Animation.h"
#include "animation/Camera.h"

namespace questless
{
	class AnimationCollection
	{
	public:
		/// Constructs an animation collection, which holds animations accessible by name.
		/// @param sprite_sheet A sprite sheet texture.
		/// @param cel_columns The number of cels in one row of the sprite sheet texture.
		/// @param cel_rows The number of cels in one column of the sprite sheet texture.
		AnimationCollection(Texture& sprite_sheet, unsigned cel_columns, unsigned cel_rows);

		AnimationCollection& operator =(const AnimationCollection&) = delete;

		/// Adds an animation to the collection.
		/// @param animation_name The name of the animation to be added.
		/// @param animation The animation to be added.
		void add(const string& animation_name, const Animation& animation);

		/// Adds an animation to the collection.
		/// @param animation_name The name of the animation to be added.
		/// @param animation The animation to be added.
		void add(const string& animation_name, Animation&& animation);

		/// Starts the given animation from the beginning.
		/// @param animation_name The name of the animation to be started.
		/// @param randomize_starting_time If true, resets the animation to a random point.
		void start(const string& animation_name, bool randomize_starting_time = false);

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
		/// @param origin The origin point of the animation on the screen.
		void draw(Point origin) const;

		/// Draws the current animation at the specified coordinates using the provided camera.
		/// @param origin The origin point of the animation on the screen.
		/// @param camera A camera object.
		void draw(Point origin, const Camera& camera) const;
	private:
		Texture& _sprite_sheet;
		int _cel_width;
		int _cel_height;

		map<string, shared_ptr<Animation>> _animations;
		shared_ptr<Animation> _current_animation;

		bool _paused;
	};
}

#endif