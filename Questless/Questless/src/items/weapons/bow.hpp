//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "reg.hpp"

#include <optional>

namespace ql {
	//! An ordinary war bow.
	struct bow {
		ent id;

		std::optional<ent> string_id;
		std::optional<ent> nocked_arrow_id;

		void nock(ent arrow_id);
		void draw();
		void loose();
	};

	ent make_bow(ent id);
}
