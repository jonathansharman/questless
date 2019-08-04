//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "body.hpp"

#include "agents/agent.hpp"
#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql {
	id make_being(id id, location location, agent agent, body body);
}
