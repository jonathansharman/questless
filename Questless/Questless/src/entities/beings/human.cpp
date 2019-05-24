//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "human.hpp"

#include "being.hpp"
#include "body.hpp"
#include "body_part.hpp"
#include "entities/entity.hpp"

namespace ql {
	void make_human(ent id, location location) {
		make_entity(id, location, transparency{0.5}, true);

		auto root = body_part{"resources/beings/human/torso.json", owner_id};
		root.generate_attached_parts();
		body body{*this, std::move(root)};

		being::stats base_stats load_base_stats_or_whatever("resources/beings/human/base_stats.json");
	}
}
