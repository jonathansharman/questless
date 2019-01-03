//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/wall_time.hpp"
#include "utility/static_bounded.hpp"

#include "world/world.hpp"

#include "sdl/spaces/colors.hpp"
#include "sdl/spaces/window.hpp"

namespace ql {
	class camera;

	//! Abstract base for animations.
	class animation {
	public:
		static constexpr double minimum_timescale = 0.0;

		//! The time scale of the animation. E.g., a time scale of 2.0 plays the animation at double speed.
		static_bounded<double, minimum_timescale> time_scale = 1.0;

		virtual ~animation() = default;

		//! Whether the current animation is paused.
		bool paused() const { return _paused; }

		//! Pauses the current animation.
		void pause() { _paused = true; }

		//! Resumes the current animation.
		void resume() { _paused = false; }

		//! Whether the animation has played to its end.
		bool over() const { return _over; }

		//! Updates the current animation.
		void update();

		//! Draws the animation at @p position in screen space.
		virtual void draw(sdl::spaces::window::point position) const = 0;

		//! Draws the animation at @p position in game space using @p camera.
		//! @param position The position in game space at which to draw the animation.
		//! @param camera The camera with which to draw the animation.
		//! @param color_factor An additional color factor, applied on top of the camera's color vector.
		virtual void draw(world::point position, camera const& camera, sdl::spaces::colors::color color_factor = sdl::spaces::colors::white()) const = 0;
	protected:
		bool _over = false;

		//! The logical amount of time elapsed since last frame, accounting for the time scale.
		sec elapsed_time() const;
	private:
		bool _paused = false;

		//! Animation subtype-specific update.
		virtual void animation_subupdate() = 0;
	};
}
