//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "body_part_status.hpp"
#include "wounds.hpp"

namespace ql {
	//! A status that expires after some time.
	struct timed_body_part_status {
		tick duration;
		body_part_status status;
	};

	//! Component for storing a being's status modifiers.
	struct body_part_status_set {
		ent id;

		//! Status modifiers that cannot be removed.
		std::vector<body_part_status> permanent;

		//! Status modifiers that remain until removed.
		std::vector<body_part_status> semipermanent;

		//! Status modifiers that expire after some time.
		std::vector<timed_body_part_status> timed;

		//! Negative status modifiers that heal over time and then expire.
		std::vector<wound> wounds;

		//! Applies the effects of this status set to its being.
		//! @param elapsed The elapsed turns since this was last applied.
		void apply(tick elapsed);
	};
}
