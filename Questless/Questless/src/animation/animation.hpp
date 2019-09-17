//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

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
		auto paused() const -> bool {
			return _paused;
		}

		//! Pauses this animation.
		auto pause() -> void {
			_paused = true;
		}

		//! Resumes this animation.
		auto resume() -> void {
			_paused = false;
		}

		//! Stops this animation.
		auto stop() -> void {
			_stopped = true;
		}

		//! Whether this animation has played to its end or been stopped early.
		auto stopped() const -> bool {
			return _stopped;
		}

		//! Advances this animation by @p elapsed_time.
		auto update(sec elapsed_time) -> void;

	protected:
		//! Marks this animation as not stopped.
		auto restart() -> void {
			_stopped = false;
		}

	private:
		bool _stopped = false;
		bool _paused = false;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

		//! Advances this animation by @p elapsed_time in a subtype-specific way.
		virtual auto animation_subupdate(sec elapsed_time) -> void = 0;

		//! Draws this animation in a subtype-specific way. The transform has already been applied to
		//! @note The @p states transform has already been multiplied by this animation's transform before this call.
		virtual auto animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void = 0;
	};
}
