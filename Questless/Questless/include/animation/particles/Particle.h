/**
* @file    Particle.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Particle class, which enables the creation of particle effects.
*/

#ifndef PARTICLE_H
#define PARTICLE_H

#include <memory>

#include "sdl-wrappers/Renderer.h"
#include "sdl-wrappers/Texture.h"
#include "animation/Camera.h"
#include "utility/Velocity.h"

namespace questless
{
	namespace ParticleK
	{
		const double max_displacement = 15.0;

		// White magic
		const Hertz white_gravity{-50.0};
		const double white_vi_max = 80.0;
		const double white_vx_factor_per_frame = 0.97;
		const double white_dtheta_max = 720.0;
		const double white_lifetime_min = 2.00;
		const double white_lifetime_max = 2.50;

		// Black magic
		const Hertz black_turn_rate{225.0};
		const double black_vxi_min = 5.0;
		const double black_vxi_max = 25.0;
		const double black_dtheta_max = 720.0;
		const Hertz black_acceleration_factor{1.25};
		const double_seconds black_lifetime{2.5};

		// Green magic
		const double green_dtheta_max = 720.0;
		const double green_v_min = 20.0;
		const double green_v_max = 50.0;
		const double green_inflection_probability = 0.1;
		const double green_turn_rate = 5.0;
		const double green_lifetime_min = 1.8;
		const double green_lifetime_max = 2.2;

		// Red magic
		const double red_v_max = 200.0;
		const double red_vyi_adjustment = -150.0;
		const double red_dtheta_max = 720.0;
		const Hertz red_gravity{300.0};
		const double red_lifetime_min = 0.6;
		const double red_lifetime_max = 1.0;

		// Blue magic
		const double blue_v = 45.0;
		const double blue_dtheta_max = 720.0;
		const double blue_lifetime_min = 2.0;
		const double blue_lifetime_max = 2.4;

		// Yellow magic
		const double yellow_v = 100.0;
		const double yellow_turn_rate_min = -60.0;
		const double yellow_turn_rate_max = 60.0;
		const double yellow_lifetime_min = 0.8;
		const double yellow_lifetime_max = 1.2;
	}

	class Particle
	{
	public:
		/// Constructs a particle with the specified properties.
		/// @param position The particle's initial position.
		/// @param velocity The particle's initial velocity.
		/// @param angle The particle's initial angle, in degrees counter-clockwise from the positive x-axis.
		/// @param angular_velocity The particle's angular velocity in degrees per second.
		/// @param lifetime The duration of the particle's life, after which it is considered expired and should be removed.
		/// @param fade_out Whether to fade the particle out as it ages.
		Particle
			( const PointF& position
			, const Velocity& velocity
			, double angle
			, Hertz angular_velocity
			, double initial_scale
			, Hertz scale_velocity
			, double_seconds lifetime
			, bool fade_out = true
			)
			: _position{position + random_displacement(ParticleK::max_displacement)}
			, _velocity{velocity}
			, _angle{angle}
			, _angular_velocity{angular_velocity}
			, _scale{initial_scale}
			, _scale_velocity{scale_velocity}
			, _lifetime{lifetime}
			, _time_left{lifetime}
			, _fade_out{fade_out}
			, _dead{false}
		{}

		/// @return Whether the particle is dead.
		bool dead() const { return _dead; }

		/// Updates the particle.
		virtual void update();

		/// Draws the particle.
		/// @param camera The camera with respect to which the particle will be drawn.
		void draw(const Camera& camera);
	protected:
		PointF _position; ///< The particle's position in game space.
		Velocity _velocity; ///< The particle's velocity in pixels per second.
		double _angle; ///< The particle's angle of rotation in radians.
		Hertz _angular_velocity; ///< The particle's angular velocity in radians per second.
		double _scale; ///< The particle's size scale.
		Hertz _scale_velocity; ///< The particle's rate of change in scale per second.
		double_seconds _lifetime; ///< The total duration in seconds of the particle's lifetime.
		double_seconds _time_left; ///< The remaining duration in seconds of the particle's lifetime.
		bool _dead; ///< False if the particle is alive / still active, true if it has already expired.
		bool _fade_out; ///< Whether the particle should fade out as it nears expiration.

		/// @todo THIS IS WRONG. Particles should be using the texture manager. Static texture pointers are not safe if the renderer has to be recreated.
		/// @return The texture to be used when drawing a particle.
		virtual Texture::ptr& texture() const = 0;
	};
}

#endif
