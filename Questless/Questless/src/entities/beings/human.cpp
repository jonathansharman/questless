//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "human.hpp"

#include "being.hpp"
#include "body.hpp"
#include "body_part.hpp"

namespace ql {
	id make_human(id id, location location, agent agent) {
		auto const root_id = reg.create();
		body_part root;
		root.id = root_id;
		root.generate_attached_parts();

		body_cond cond;
		stats::body base_stats; // load_base_stats_or_whatever("resources/beings/human/base_stats.json");
		body body{id, root.id, cond, base_stats};

		make_being(id, location, agent, body);

		return id;
	}
}
