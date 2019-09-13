//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "../sphere.hpp"

namespace vecx {
	//! cereal-compatible save function for spheres.
	template <typename Archive, typename Quantity, std::size_t N>
	auto save(Archive& archive, sphere<Quantity, N> const& box) -> void {
		archive(sphere.center, sphere.radius);
	}

	//! cereal-compatible load function for spheres.
	template <typename Archive, typename Quantity, std::size_t N>
	auto load(Archive& archive, sphere<Quantity, N>& box) -> void {
		archive(sphere.center, sphere.radius);
	}
}
