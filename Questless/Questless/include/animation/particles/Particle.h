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

#include "sdl-wrappers/Renderer.h"
#include "sdl-wrappers/Texture.h"
#include "animation/Camera.h"
#include "utility/Velocity.h"

namespace questless
{
	class Particle
	{
	public:
		/// Constructs a particle with the specified properties.
		/// @param position The particle's initial position.
		/// @param velocity The particle's initial velocity.
		/// @param angle The particle's initial angle, in degrees counter-clockwise from the positive x-axis.
		/// @param angular_velocity The particle's angular velocity in degrees per second.
		/// @param lifetime The duration of the particle's life, after which it is considered expired and should be removed.
		Particle
			( const PointF& position
			, const Velocity& velocity
			, double angle
			, Hertz angular_velocity
			, double initial_scale
			, Hertz scale_velocity
			, double_seconds lifetime
			)
			: _position{position + random_displacement(max_displacement)}
			, _velocity{velocity}
			, _angle{angle}
			, _angular_velocity{angular_velocity}
			, _scale{initial_scale}
			, _scale_velocity{scale_velocity}
			, _lifetime{lifetime}
			, _time_left{lifetime}
		{}

		/// @return Whether the particle is dead.
		bool dead() const { return _time_left < double_seconds::zero(); }

		/// Updates the particle.
		void update();

		/// Draws the particle.
		/// @param camera The camera with respect to which the particle will be drawn.
		void draw(const Camera& camera);
	protected:
		/// @todo Are these protected variables the right way to do this?

		PointF _position; ///< The particle's position in game space.
		Velocity _velocity; ///< The particle's velocity in pixels per second.
		double _angle; ///< The particle's angle of rotation in radians.
		Hertz _angular_velocity; ///< The particle's angular velocity in radians per second.
		double _scale; ///< The particle's size scale.
		Hertz _scale_velocity; ///< The particle's rate of change in scale per second.
		double_seconds _lifetime; ///< The total duration in seconds of the particle's lifetime.
		double_seconds _time_left; ///< The remaining duration in seconds of the particle's lifetime.

		/// @return The texture to be used when drawing a particle.
		virtual sdl::Texture& texture() const = 0;
	private:
		static constexpr double max_displacement = 15.0;

		/// @return Whether the particle should fade out as it nears expiration.
		virtual bool fade_out() const { return true; };

		/// Performs any updates specific to the particle subclass.
		virtual void subupdate() = 0;
	};
}

#endif
