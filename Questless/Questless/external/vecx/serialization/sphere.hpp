//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "../sphere.hpp"

namespace vecx {
	//! cereal-compatible save function for spheres.
	template <typename Archive, typename Quantity, std::size_t N>
	void save(Archive& archive, sphere<Quantity, N> const& box) {
		archive(sphere.center, sphere.radius);
	}

	//! cereal-compatible load function for spheres.
	template <typename Archive, typename Quantity, std::size_t N>
	void load(Archive& archive, sphere<Quantity, N>& box) {
		archive(sphere.center, sphere.radius);
	}
}
