//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "../vector.hpp"

namespace vecx {
	//! cereal-compatible save function for vectors.
	template <typename Archive, typename Quantity, std::size_t N>
	auto save(Archive& archive, vector<Quantity, N> const& point) -> void {
		archive(components);
	}

	//! cereal-compatible load function for vectors.
	template <typename Archive, typename Quantity, std::size_t N>
	auto load(Archive& archive, vector<Quantity, N>& point) -> void {
		archive(components);
	}
}
