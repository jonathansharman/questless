//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "body_cond.hpp"
#include "body_status_set.hpp"
#include "stats/body.hpp"

#include "quantities/misc.hpp"
#include "reg.hpp"

#include <functional>

namespace ql {
	struct body_part;

	//! A being's body, which is composed of a tree of body parts.
	struct body {
		ent id;

		//! The ID of the root body part, to which all other body parts are attached.
		ent root_part_id;

		//! This body's conditions.
		body_cond cond;

		//! This body's base stats, before any stat-modifying effects are applied.
		stats::body base_stats;

		//! This body's current stats.
		stats::body stats;

		body_status_set status_set;

		body(ent id, ent root_part_id, body_cond cond, stats::body base_stats);

		//! Performs @p f for each body part in this body. See also @p for_enabled_parts.
		void for_all_parts(std::function<void(body_part const&)> const& f) const;
		//! Performs @p f for each body part in this body. See also @p for_enabled_parts.
		void for_all_parts(std::function<void(body_part&)> const& f);

		//! Performs @p f for each enabled body part in this body. See also @p for_all_parts.
		void for_enabled_parts(std::function<void(body_part const&)> const& f) const;
		//! Performs @p f for each enabled body part in this body. See also @p for_all_parts.
		void for_enabled_parts(std::function<void(body_part&)> const& f);

		//! Advances this body and all its parts by @p elapsed.
		void update(tick elapsed);
	};
}
