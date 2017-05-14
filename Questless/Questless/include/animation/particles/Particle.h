//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "../Animation.h"
#include "../Camera.h"
#include "sdl/Texture.h"
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

	//! Used to create particle effects.
	class Particle : public Animation
	{
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
		Particle
			( units::GameVector displacement
			, units::GameVelocity velocity
			, units::GameAcceleration acceleration
			, units::GameRadians angle
			, units::GameRadiansPerSec angular_velocity
			, Scale scale
			, units::GameScaleVelocity scale_velocity
			, Lifetime lifetime
			, MaxDisplacement max_displacement = MaxDisplacement{_dflt_max_displacement}
			)
			: _displacement{displacement + random_displacement(max_displacement)}
			, _velocity{std::move(velocity)}
			, _acceleration{std::move(acceleration)}
			, _angle{std::move(angle)}
			, _angular_velocity{std::move(angular_velocity)}
			, _scale{scale}
			, _scale_velocity{scale_velocity}
			, _lifetime{lifetime}
			, _time_left{lifetime}
			, _color{sdl::Color::white()}
		{}

		virtual ~Particle() = default;

		void draw(units::ScreenPoint position) const final;

		void draw(units::GamePoint position, Camera const& camera, sdl::Color color = sdl::Color::white()) const final;
	protected:
		//! @todo Are these protected variables the right way to do this?

		units::GameVector _displacement;
		units::GameVelocity _velocity;
		units::GameAcceleration _acceleration;
		units::GameRadians _angle;
		units::GameRadiansPerSec _angular_velocity;
		double _scale;
		units::GameScaleVelocity _scale_velocity;
		units::GameSeconds _lifetime;
		units::GameSeconds _time_left;
		sdl::Color _color;

		//! The texture to be used when drawing a particle.
		virtual sdl::Texture const& texture() const = 0;
	private:
		static constexpr double _dflt_max_displacement = 15.0;

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
