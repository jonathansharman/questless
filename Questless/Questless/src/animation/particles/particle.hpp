//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/animation.hpp"

#include "cancel/quantity.hpp"

#include "sdl/texture.hpp"

namespace ql {
	using max_displacement = cancel::quantity<double, cancel::unit_t<struct max_displacement_tag>>;

	class camera;

	//! Used to create particle effects.
	class particle : public animation {
	public:
		//! @param displacement Initial displacement from the origin, in game space.
		//! @param velocity Initial velocity in game pixels per second.
		//! @param acceleration Initial acceleration in game pixels per second per second.
		//! @param angle Initial angle, in radians, counter-clockwise from the positive x-axis.
		//! @param angular_velocity Initial angular velocity in radians per second.
		//! @param scale Initial size scale.
		//! @param scale_velocity Initial rate of change in size scale per second.
		//! @param lifetime The duration of the particle's life, after which it is considered expired and should be removed.
		//! @param max_displacement Maximum random displacement from the given position.
		particle
			( world::vector displacement
			, world::vel velocity
			, world::accel acceleration
			, vecx::radians angle
			, world::angular_vel angular_velocity
			, double scale
			, per_sec scale_velocity
			, sec lifetime
			, max_displacement max_displacement = max_displacement{15.0}
			);

		virtual ~particle() = default;

		void draw(sdl::spaces::window::point position) const final;

		void draw(world::point position, camera const& camera, sdl::spaces::colors::color color_factor = sdl::spaces::colors::white()) const final;
	protected:
		//! @todo Are these protected variables the right way to do this?

		world::vector _displacement;
		world::vel _velocity;
		world::accel _acceleration;
		vecx::radians _angle;
		world::angular_vel _angular_velocity;
		double _scale;
		per_sec _scale_velocity;
		sec _lifetime;
		sec _time_left;
		sdl::spaces::colors::color _color_factor;

		//! The texture to be used when drawing a particle.
		virtual sdl::texture const& texture() const = 0;
	private:
		//! Whether the particle should fade out as it nears expiration.
		virtual bool fade_out() const { return true; };

		//! Whether the particle's angle should be set to match its velocity.
		virtual bool face_towards_heading() const { return false; }

		//! Whether the particle should ignore the draw() color mod.
		virtual bool ignore_color_mod() const { return true; }

		//! Animation subtype-specific update.
		void animation_subupdate() final;

		//! Particle subtype-specific update.
		virtual void particle_subupdate() = 0;
	};
}
