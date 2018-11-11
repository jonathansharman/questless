//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "../point.hpp"

namespace vecx {
	//! cereal-compatible save function for points.
	template <typename Archive, typename Quantity, std::size_t N>
	void save(Archive& archive, point<Quantity, N> const& point) {
		archive(components);
	}

	//! cereal-compatible load function for points.
	template <typename Archive, typename Quantity, std::size_t N>
	void load(Archive& archive, point<Quantity, N>& point) {
		archive(components);
	}
}
