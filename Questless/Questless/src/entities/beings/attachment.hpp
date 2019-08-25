//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "body_part_generator.hpp"

#include "reg.hpp"
#include "ui/view_space.hpp"

#include "vecx/angle.hpp"

#include <optional>

namespace ql {
	//! An attachment point on a body part.
	struct attachment {
		//! The ID of the being that owns this body part attachment.
		id owner_id;

		//! The body part currently attached here or nullopt if there is no attached part.
		std::optional<id> o_part_id;

		//! Body part generator for the type of part that attaches here by default.
		generators::generator default_generator;
	};
}
