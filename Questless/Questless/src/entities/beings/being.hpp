//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "body.hpp"

#include "agents/agent.hpp"
#include "entities/transparency.hpp"
#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql {
	void make_being(ent id, location location, agent agent, body body, transparency transparency);
}
