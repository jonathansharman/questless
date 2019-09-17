//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "body.hpp"

#include "agents/agent.hpp"
#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql {
	auto make_being(id being_id, location location, agent agent, body body) -> id;
}
