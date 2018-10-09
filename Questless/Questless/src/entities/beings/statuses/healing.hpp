//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/status.hpp"

namespace ql {
	//! Causes a being to gain health over time.
	class healing : public status {
	public:
		//! @param rate The rate of healing.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		healing(health_per_tick rate, tick duration, std::optional<id<being>> source_id = std::nullopt)
			: status("Healing", duration, source_id), _rate{rate}
		{}

		virtual status_type type() const { return status_type::boon; }

		void subupdate(being& target, tick elapsed) final;
	private:
		ql::health_per_tick _rate;
	};
}
