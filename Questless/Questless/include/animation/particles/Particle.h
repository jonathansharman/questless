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
#include "utility/units/vectors.h"

namespace questless
{
	using Lifetime = TaggedType<seconds_f, struct LifetimeTag>;
	using MaxDisplacement = TaggedType<double, struct MaxDisplacementTag>;

	class Particle
	{
	public:
		using ptr = std::unique_ptr<Particle>;

		/// @param position Initial position in game space.
		/// @param velocity Initial velocity in game pixels per second.
		/// @param acceleration Initial acceleration in game pixels per second per second.
		/// @param angle Initial angle, in radians, counter-clockwise from the positive x-axis.
		/// @param angular_velocity Initial angular velocity in radians per second.
		/// @param scale Initial size scale.
		/// @param scale_velocity Initial rate of change in size scale per second.
		/// @param lifetime The duration of the particle's life, after which it is considered expired and should be removed.
		Particle
			( PointF position
			, Velocity velocity
			, Acceleration acceleration
			, AngleRadians angle
			, AngularVelocity angular_velocity
			, Scale scale
			, ScaleVelocity scale_velocity
			, Lifetime lifetime
			, MaxDisplacement max_displacement = MaxDisplacement{_dflt_max_displacement}
			)
			: _position{position + random_displacement(max_displacement)}
			, _velocity{velocity}
			, _acceleration{acceleration}
			, _angle{angle}
			, _angular_velocity{angular_velocity}
			, _scale{scale}
			, _scale_velocity{scale_velocity}
			, _lifetime{lifetime}
			, _time_left{lifetime}
		{}

		/// @return Whether the particle is dead.
		bool dead() const { return _time_left < seconds_f::zero(); }

		/// Updates the particle.
		void update();

		/// Draws the particle.
		/// @param camera The camera with respect to which the particle will be drawn.
		void draw(const Camera& camera);
	protected:
		/// @todo Are these protected variables the right way to do this?

		PointF _position;
		Velocity _velocity;
		Acceleration _acceleration;
		double _angle;
		AngularVelocity _angular_velocity;
		double _scale;
		ScaleVelocity _scale_velocity;
		seconds_f _lifetime;
		seconds_f _time_left;

		/// @return The texture to be used when drawing a particle.
		virtual sdl::Texture& texture() const = 0;
	private:
		static constexpr double _dflt_max_displacement = 15.0;

		/// @return Whether the particle should fade out as it nears expiration.
		virtual bool fade_out() const { return true; };

		/// @return Whether the particle's angle should be set to match its velocity.
		virtual bool face_towards_heading() const { return false; }

		/// Performs any updates specific to the particle subclass.
		virtual void subupdate() = 0;
	};
}

#endif
