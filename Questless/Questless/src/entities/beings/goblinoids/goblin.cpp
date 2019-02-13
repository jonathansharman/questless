//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/goblinoids/goblin.hpp"
#include "entities/beings/body.hpp"
#include "entities/beings/body_part.hpp"

namespace ql {
	body goblin::make_body(ql::id<being> owner_id) const {
		auto root = body_part{"resources/beings/goblin/parts/torso.json", owner_id};
		root.generate_attached_parts();
		return ql::body{*this, std::move(root)};
	}

	stats::being goblin::make_base_stats() const {
		return load_stats("resources/beings/goblin/base_stats.json");
	}
}
