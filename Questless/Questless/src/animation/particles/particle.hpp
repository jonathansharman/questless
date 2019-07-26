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
		world::vector displacement = world::vector::zero();

		world::vel velocity = world::vel::zero();

		world::accel acceleration = world::accel::zero();

		vecx::radians angle = vecx::radians{0.0};

		world::angular_vel angular_velocity = vecx::radians{0.0} / 0.0_s;

		cancel::unitless<float> scale{1.0f};

		per_sec scale_velocity = 0.0_hz;

		sf::Color color_factor = sf::Color::White;

		virtual ~particle() = default;

		void animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		//! Whether the particle should fade out as it nears expiration.
		virtual bool fade_out() const {
			return true;
		};

		//! Whether the particle's angle should be set to match its velocity.
		virtual bool face_towards_heading() const {
			return false;
		}

		void animation_subupdate(sec elapsed_time) final;

		//! Updates the particle in a subtype-specific way.
		virtual void particle_subupdate(sec elapsed_time) = 0;

		//! Draws the particle in a subtype-specific way.
		virtual void particle_subdraw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	};
}
