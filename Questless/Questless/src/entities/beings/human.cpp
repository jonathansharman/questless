//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "human.hpp"

#include "being.hpp"
#include "body.hpp"
#include "body_part.hpp"

namespace ql {
	body human::make_body(ql::id<being> owner_id) const {
		auto root = body_part{"resources/beings/human/torso.json", owner_id};
		root.generate_attached_parts();
		return ql::body{*this, std::move(root)};
	}

	stats::being human::make_base_stats() const {
		return load_stats("resources/beings/human/base_stats.json");
	}
}
