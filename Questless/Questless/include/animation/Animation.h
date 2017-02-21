/**
* @file    Animation.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Provides basic 2D animation.
*/

#pragma once

#include <vector>
#include <string>
#include <chrono>

#include <boost/optional.hpp>

#include "units/SpriteSheetPoint.h"
#include "units/TexturePoint.h"
#include "units/GameSeconds.h"
#include "utility/TaggedType.h"

namespace questless
{
	struct Looping : TaggedType<bool> { using TaggedType::TaggedType; };
	struct RandomizeStartTime : TaggedType<bool> { using TaggedType::TaggedType; };

	class Animation
	{
	public:
		struct Frame
		{
			units::GameSeconds duration;
			units::SpriteSheetPoint coords; ///< The cel coordinates within the sprite sheet.
			units::TexturePoint origin; ///< The origin of the frame's texture relative to the animation origin.

			Frame(units::GameSeconds duration, units::SpriteSheetPoint coords, units::TexturePoint origin)
				: duration{duration}, coords{coords}, origin{origin}
			{}
		};

		/// Constructs an animation object from the provided frames using the provided loop type.
		/// @param frames The sequence of frames that compose the animation.
		/// @param looping Whether to loop the animation or play just once.
		Animation(std::vector<Frame> frames, Looping looping);

		/// @return Whether the animation is looping.
		bool looping() const { return _looping; }

		/// @param looping Whether to loop the animation or play just once.
		void looping(bool looping) { _looping = looping; }

		/// @return The number of times the animation has looped.
		int loops() const { return _loops; }

		/// @return The current frame of the animation.
		Frame const& current_frame() const { return _frames[_frame_index]; }

		/// @return The time scale.
		double time_scale() const { return _time_scale; }

		/// @param time_scale A non-negative double to which the time scale is to be set. If time_scale is negative, the time scale is set to 0.
		void time_scale(double time_scale) { _time_scale = time_scale < 0 ? 0 : time_scale; }

		/// @return Whether the animation has played to its end.
		bool over() const { return _over; }

		/// @return The total duration of the animation.
		units::GameSeconds duration() const;

		/// Moves to the start or a random time point in the animation, sets the loop counter to zero, and sets the over flag to false.
		/// @param randomize_start_time If true, resets the animation to a random time point.
		void reset(RandomizeStartTime randomize_start_time = RandomizeStartTime{false});

		/// Advances the animation. Should be called once per game update.
		void update();
	private:
		std::vector<Frame> _frames;
		int _frame_index;
		units::GameSeconds _accrued_time;
		bool _looping;
		int _loops;
		bool _in_reverse;
		double _time_scale;
		bool _over;
	};
}
