//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/static_bounded.hpp"

#include "cancel/quantity.hpp"

namespace ql::perception {
	//! The degree to which a being perceives an entity.
	using level = cancel::quantity<double, cancel::unit_t<struct perception_leve_tag>>;

	constexpr auto minimum_level = level{0.0};
	constexpr auto maximum_level = level{100.0};

	using bounded_level = static_bounded<perception::level, perception::minimum_level, perception::maximum_level>;

	//! The category of a being's perception of an entity.
	enum class category
		{ none   //!< Unperceived.
		, low    //!< Presence perceived.
		, medium //!< Type of entity perceived.
		, high   //!< Attributes such as health and equipped items perceived.
		, full   //!< All attributes perceived.
		};

	//! Maps perception level to perception category.
	constexpr category get_category(level level) {
		if (level > perception::level{75.0}) {
			return category::full;
		} else if (level > perception::level{50.0}) {
			return category::high;
		} else if (level > perception::level{25.0}) {
			return category::medium;
		} else if (level > perception::level{0.0}) {
			return category::low;
		} else {
			return category::none;
		}
	}
}

namespace ql {
	constexpr perception::level operator "" _perception(long double value) { return perception::level{static_cast<double>(value)}; }
}
