//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded/nonnegative.hpp"
#include "quantities/wall_time.hpp"
#include "ui/view_space.hpp"

#include <SFML/Graphics.hpp>

namespace ql {
	//! Abstract base for animations.
	struct animation
		: public sf::Drawable
		, public sf::Transformable //
	{
		//! The time scale of the animation. E.g., a time scale of 2.0 plays the animation at double speed.
		nonnegative<float> time_scale = 1.0;

		virtual ~animation() = default;

		//! Whether this animation is paused.
		bool paused() const {
			return _paused;
		}

		//! Pauses this animation.
		void pause() {
			_paused = true;
		}

		//! Resumes this animation.
		void resume() {
			_paused = false;
		}

		//! Stops this animation.
		void stop() {
			_stopped = true;
		}

		//! Whether this animation has played to its end or been stopped early.
		bool stopped() const {
			return _stopped;
		}

		//! Advances this animation by @p elapsed_time.
		void update(sec elapsed_time);

	protected:
		//! Marks this animation as not stopped.
		void restart() {
			_stopped = false;
		}

	private:
		bool _stopped = false;
		bool _paused = false;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

		//! Advances this animation by @p elapsed_time in a subtype-specific way.
		virtual void animation_subupdate(sec elapsed_time) = 0;

		//! Draws this animation in a subtype-specific way. The transform has already been applied to
		//! @note The @p states transform has already been multiplied by this animation's transform before this call.
		virtual void animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	};
}
