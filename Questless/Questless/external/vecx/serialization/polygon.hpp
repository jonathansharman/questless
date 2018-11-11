//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "../polygon.hpp"

namespace vecx {
	//! cereal-compatible save function for polygons.
	template <typename Archive, typename Quantity>
	void save(Archive& archive, polygon<Quantity> const& polygon) {
		archive(polygon.vertices);
	}

	//! cereal-compatible load function for polygons.
	template <typename Archive, typename Quantity>
	void load(Archive& archive, polygon<Quantity>& polygon) {
		archive(polygon.vertices);
	}
}
