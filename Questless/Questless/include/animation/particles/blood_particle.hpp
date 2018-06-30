//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"

namespace ql
{
	//! A blood drop particle.
	class blood_particle : public particle
	{
	public:
		blood_particle();
	private:
		bool fade_out() const final { return false; };

		bool face_towards_heading() const final { return true; }

		virtual bool ignore_color_mod() const { return false; }

		void particle_subupdate() final {}

		sdl::texture const& texture() const final;
	};
}
