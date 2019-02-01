//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "particle_animation.hpp"

namespace ql {
	//! Creates drops of blood.
	class bleeding : public particle_animation {
	public:
		//! Unit of drops of blood, for use in bleeding animations.
		using drops = cancel::quantity<double, cancel::unit_t<struct bleeding_drops_tag>>;

		//! The number of drops of blood to create per second. Can be less than one.
		decltype(drops{} / sec{}) drop_rate;

		//! The number of drops of blood to create per second. Can be less than one.
		bleeding(decltype(drops{} / sec{}) drop_rate) : drop_rate{drop_rate} {}
	private:
		//! The current accumulation of drops of blood.
		drops _drops{0.0};

		void particle_animation_subupdate(sec elapsed_time) final;
	};
}
