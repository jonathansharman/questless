//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/human.hpp"
#include "entities/beings/body.hpp"
#include "entities/beings/body_part.hpp"

using namespace units;

namespace ql {
	human::human(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id)
		: being_base<human>
			{ make_agent
			, id
			, make_body(id)
			, [] { return load_stats("resources/beings/human/base_stats.json"); }
			}
	{}

	body human::make_body(ql::id<being> owner_id) {
		auto root = body_part{"resources/beings/human/torso.json", owner_id};
		root.generate_attached_parts();
		return ql::body{*this, std::move(root)};
	}
}
