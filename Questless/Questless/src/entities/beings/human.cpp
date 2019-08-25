//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "human.hpp"

#include "being.hpp"
#include "body.hpp"
#include "body_part.hpp"
#include "body_part_generator.hpp"

namespace ql {
	id make_human(id id, location location, agent agent) {
		ql::id root_id = generators::human::torso{}.make(id);
		auto& root = reg.get<body_part>(root_id);
		root.generate_attached_parts();

		make_being(id, location, agent, body{id, root_id, body_cond{}, generators::human::make_body_stats()});

		return id;
	}
}
