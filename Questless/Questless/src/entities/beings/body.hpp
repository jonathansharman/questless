//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

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
		reg_ptr reg;

		id id;

		//! The ID of the root body part, to which all other body parts are attached.
		ql::id root_part_id;

		body_cond cond;

		stats::body stats;

		body_status_set status_set;

		body(ql::reg& reg, ql::id id, ql::id root_part_id, body_cond cond, stats::body stats);

		//! Performs @p f for each body part in this body. See also @p for_enabled_parts.
		auto for_all_parts(std::function<void(body_part const&)> const& f) const -> void;
		//! Performs @p f for each body part in this body. See also @p for_enabled_parts.
		auto for_all_parts(std::function<void(body_part&)> const& f) -> void;

		//! Performs @p f for each enabled body part in this body. See also @p for_all_parts.
		auto for_enabled_parts(std::function<void(body_part const&)> const& f) const -> void;
		//! Performs @p f for each enabled body part in this body. See also @p for_all_parts.
		auto for_enabled_parts(std::function<void(body_part&)> const& f) -> void;

		//! Advances this body and all its parts by @p elapsed.
		auto update(tick elapsed) -> void;
	};
}
