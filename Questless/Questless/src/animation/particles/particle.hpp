//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/animation.hpp"

#include "cancel/quantity.hpp"

namespace ql {
	//! Used to create particle effects.
	struct particle : animation {
		//! @param lifetime The amount of time before this particle expires.
		particle(sec lifetime);

		//! The total length of time the particle lives. Not to be confused with @p time_left.
		sec lifetime;

		//! The amount of time left before this particle expires.
		sec time_left = lifetime;

		//! Displacement of this particle from its animation position.
		view::vector displacement{};

		view::vel velocity{};

		view::accel acceleration{};

		vecx::radians angle = vecx::radians{0.0};

		view::angular_vel angular_velocity = vecx::radians{0.0} / 1.0_s;

		cancel::unitless<float> scale{1.0f};

		per_sec scale_velocity = 0.0_hz;

		sf::Color color_factor = sf::Color::White;

		virtual ~particle() = default;

		auto animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

	private:
		//! Whether the particle should fade out as it nears expiration.
		virtual auto fade_out() const -> bool {
			return true;
		};

		//! Whether the particle's angle should be set to match its velocity.
		virtual auto face_towards_heading() const -> bool {
			return false;
		}

		auto animation_subupdate(sec elapsed_time) -> void final;

		//! Updates the particle in a subtype-specific way.
		virtual auto particle_subupdate(sec elapsed_time) -> void = 0;

		//! Draws the particle in a subtype-specific way.
		virtual auto particle_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void = 0;
	};
}
