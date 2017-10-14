//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "../animation.h"
#include "../camera.h"
#include "sdl/texture.h"
#include "utility/tagged_type.h"

namespace ql
{
	struct scale : tagged_type<double> { using tagged_type::tagged_type; };
	struct lifetime : tagged_type<units::game_space::seconds> { using tagged_type::tagged_type; };
	struct max_displacement : tagged_type<double> { using tagged_type::tagged_type; };

	//! Used to create particle effects.
	class particle : public animation
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
		particle
		(units::game_space::vector displacement
			, units::game_space::velocity velocity
			, units::game_space::acceleration acceleration
			, units::game_space::radians angle
			, units::game_space::radians_per_sec angular_velocity
			, scale scale
			, units::game_space::scale_velocity scale_velocity
			, lifetime lifetime
			, max_displacement max_displacement = max_displacement{_dflt_max_displacement}
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
			, _color_factor{units::colors::white_factor()}
		{}

		virtual ~particle() = default;

		void draw(units::screen_space::point position) const final;

		void draw(units::game_space::point position, camera const& camera, units::colors::color_factor color_factor = units::colors::white_factor()) const final;
	protected:
		//! @todo Are these protected variables the right way to do this?

		units::game_space::vector _displacement;
		units::game_space::velocity _velocity;
		units::game_space::acceleration _acceleration;
		units::game_space::radians _angle;
		units::game_space::radians_per_sec _angular_velocity;
		double _scale;
		units::game_space::scale_velocity _scale_velocity;
		units::game_space::seconds _lifetime;
		units::game_space::seconds _time_left;
		units::colors::color_factor _color_factor;

		//! The texture to be used when drawing a particle.
		virtual sdl::texture const& texture() const = 0;
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
