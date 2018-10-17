//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/goblinoids/goblin.hpp"
#include "entities/beings/body.hpp"
#include "entities/beings/body_part.hpp"

using namespace units;

namespace ql {
	goblin::goblin(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id)
		: being_base<goblin>
			{ make_agent
			, id
			, make_body(id)
			, [] { return load_stats("resources/beings/goblin/base_stats.json"); }
			}
	{}

	body goblin::make_body(ql::id<being> owner_id) {
		auto root = body_part{"resources/beings/goblin/parts/torso.json", owner_id};
		root.generate_attached_parts();
		return ql::body{*this, std::move(root)};
	}
}
