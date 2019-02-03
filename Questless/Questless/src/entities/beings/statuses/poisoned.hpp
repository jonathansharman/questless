//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/status.hpp"

namespace ql {
	//! Deals blight damage over time.
	struct poisoned : status {
		//! @param rate The rate of blight damage.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		poisoned(cancel::quotient_t<dmg::blight, tick> rate, tick duration, std::optional<id<being>> source_id = std::nullopt)
			: status("Poisoned", duration, source_id), _rate{rate} {}

		virtual status_type type() const {
			return status_type::bane;
		}

		void subupdate(being& target, tick elapsed) final;

	private:
		cancel::quotient_t<dmg::blight, tick> _rate;
	};
}
