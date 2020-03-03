//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "human.hpp"

#include "being.hpp"
#include "body.hpp"
#include "body_part.hpp"
#include "body_part_generator.hpp"

namespace ql {
	id make_human(reg& reg, id human_id, location location, agent agent) {
		auto root_id = generators::human::torso{}.make(reg, human_id);
		auto& root = reg.get<body_part>(root_id);
		root.generate_attached_parts();

		make_being(reg, human_id, location, agent, body{reg, human_id, root_id, body_cond{}, generators::human::make_body_stats()});

		return human_id;
	}
}
