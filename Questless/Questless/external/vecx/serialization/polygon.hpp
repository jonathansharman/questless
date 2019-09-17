//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "../polygon.hpp"

namespace vecx {
	//! cereal-compatible save function for polygons.
	template <typename Archive, typename Quantity>
	auto save(Archive& archive, polygon<Quantity> const& polygon) -> void {
		archive(polygon.vertices);
	}

	//! cereal-compatible load function for polygons.
	template <typename Archive, typename Quantity>
	auto load(Archive& archive, polygon<Quantity>& polygon) -> void {
		archive(polygon.vertices);
	}
}
