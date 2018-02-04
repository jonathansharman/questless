//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"
#include "units/game_space.hpp"

namespace ql
{
	//! An arrow in flight.
	class arrow_particle : public particle
	{
	public:
		//! @param source Where the arrow is shot from.
		//! @param target Where the arrow is headed towards.
		arrow_particle(units::game_space::point source, units::game_space::point target);
	private:
		units::game_space::point _target;

		bool fade_out() const final { return false; };

		bool face_towards_heading() const final { return true; }

		virtual bool ignore_color_mod() const { return false; }

		void particle_subupdate() final {}

		sdl::texture const& texture() const final;
	};
}
