//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "../box.hpp"

namespace vecx {
	//! cereal-compatible save function for boxes.
	template <typename Archive, typename Quantity, std::size_t N>
	auto save(Archive& archive, box<Quantity, N> const& box) -> void {
		archive(box.position, box.size);
	}

	//! cereal-compatible load function for boxes.
	template <typename Archive, typename Quantity, std::size_t N>
	auto load(Archive& archive, box<Quantity, N>& box) -> void {
		archive(box.position, box.size);
	}
}
