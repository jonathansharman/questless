//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.hpp"

namespace ql {
	//! A particle of flame and smoke.
	class flame_particle : public particle {
	public:
		flame_particle();
	private:
		static constexpr per_sec _vx_pct_drag_rate = 1.8_hz;

		void particle_subupdate() final;

		sdl::texture const& texture() const final;
	};
}
