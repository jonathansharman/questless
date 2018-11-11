//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "../vector.hpp"

namespace vecx {
	//! cereal-compatible save function for vectors.
	template <typename Archive, typename Quantity, std::size_t N>
	void save(Archive& archive, vector<Quantity, N> const& point) {
		archive(components);
	}

	//! cereal-compatible load function for vectors.
	template <typename Archive, typename Quantity, std::size_t N>
	void load(Archive& archive, vector<Quantity, N>& point) {
		archive(components);
	}
}
