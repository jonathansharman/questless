/**
* @file    Particle.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Particle class, which enables the creation of particle effects.
*/

#pragma once

#include "sdl/Renderer.h"
#include "sdl/Texture.h"
#include "animation/Camera.h"
#include "units/GameSeconds.h"
#include "units/GameVelocity.h"
#include "units/GameAcceleration.h"
#include "units/GameRadiansPerSec.h"
#include "units/GameScaleVelocity.h"
#include "utility/TaggedType.h"

namespace questless
{
	struct Scale : TaggedType<double> { using TaggedType::TaggedType; };
	struct Lifetime : TaggedType<units::GameSeconds> { using TaggedType::TaggedType; };
	struct MaxDisplacement : TaggedType<double> { using TaggedType::TaggedType; };

	class Particle
	{
	public:
		using uptr = std::unique_ptr<Particle>;

		/// @param position Initial position in game space.
		/// @param velocity Initial velocity in game pixels per second.
		/// @param acceleration Initial acceleration in game pixels per second per second.
		/// @param angle Initial angle, in radians, counter-clockwise from the positive x-axis.
		/// @param angular_velocity Initial angular velocity in radians per second.
		/// @param scale Initial size scale.
		/// @param scale_velocity Initial rate of change in size scale per second.
		/// @param lifetime The duration of the particle's life, after which it is considered expired and should be removed.
		/// @param max_displacement Maximum random displacement from the given position.
		////
		Particle
			( units::GamePoint position
			, units::GameVelocity velocity
			, units::GameAcceleration acceleration
			, units::GameRadians angle
			, units::GameRadiansPerSec angular_velocity
			, Scale scale
			, units::GameScaleVelocity scale_velocity
			, Lifetime lifetime
			, MaxDisplacement max_displacement = MaxDisplacement{_dflt_max_displacement}
			)
			: _position{position + random_displacement(max_displacement)}
			, _velocity{std::move(velocity)}
			, _acceleration{std::move(acceleration)}
			, _angle{std::move(angle)}
			, _angular_velocity{std::move(angular_velocity)}
			, _scale{scale}
			, _scale_velocity{scale_velocity}
			, _lifetime{lifetime}
			, _time_left{lifetime}
		{}

		virtual ~Particle() = default;

		/// @return Whether the particle is expired.
		////
		bool expired() const { return _time_left < units::GameSeconds::zero(); }

		/// Updates the particle.
		////
		void update();

		/// Draws the particle.
		/// @param camera The camera with respect to which the particle will be drawn.
		////
		void draw(Camera const& camera);
	protected:
		/// @todo Are these protected variables the right way to do this?

		units::GamePoint _position;
		units::GameVelocity _velocity;
		units::GameAcceleration _acceleration;
		units::GameRadians _angle;
		units::GameRadiansPerSec _angular_velocity;
		double _scale;
		units::GameScaleVelocity _scale_velocity;
		units::GameSeconds _lifetime;
		units::GameSeconds _time_left;

		/// @return The texture to be used when drawing a particle.
		virtual sdl::Texture const& texture() const = 0;
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
