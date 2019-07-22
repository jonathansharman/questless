//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "body_status.hpp"

namespace ql {
	//! A status that expires after some time.
	struct timed_body_status {
		body_status status;
		tick duration;
	};

	//! Component for storing a being's status modifiers.
	struct body_status_set {
		ent id;

		//! Status modifiers that cannot be removed.
		std::vector<body_status> permanent;

		//! Status modifiers that remain until removed.
		std::vector<body_status> semipermanent;

		//! Status modifiers that expire after some time.
		std::vector<timed_body_status> timed;

		//! Applies the effects of this status set to its being.
		//! @param elapsed The elapsed turns since this was last applied.
		void apply(tick elapsed);
	};
}
