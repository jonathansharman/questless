//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "reg.hpp"

#include <optional>

namespace ql {
	//! An ordinary war bow.
	struct bow {
		id id;

		std::optional<ql::id> string_id;
		std::optional<ql::id> nocked_arrow_id;

		void nock(ql::id arrow_id);
		void draw();
		void loose();
	};

	id make_bow(id bow_id);
}
