//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "point.hpp"
#include "vector.hpp"

namespace units {
	//! Represents the axis along the dimension with index @p Index.
	template <std::size_t Index>
	struct axis {
		static_assert(0 <= Index && Index < n, "Axis index out of bounds.");

		//! Alignment along this axis.
		enum class alignment { near, mid, far };
	};

	//! @todo Is there a way to generate the type for the alignment tuple without using a dummy function?

	template <std::size_t... DimensionIndices>
	static decltype(auto) align_dummy(std::index_sequence<DimensionIndices...>) {
		return std::tuple<typename axis<DimensionIndices>::align...>{};
	}

	//! The alignment along every axis in this space.
	template <std::size_t N>
	using alignment = decltype(align_dummy(std::make_index_sequence<N>{}));

	template <typename... Quantities>
	struct box {
		//! The number of dimensions of this box type.
		static constexpr std::size_t n = sizeof...(Quantities);

		//! The position of the minimal corner of this box.
		point<Quantities...> position;

		//! The size or extent of this box.
		vector<Quantities...> size;

		template <typename Archive>
		void save(Archive& archive) const {
			archive(position, size);
		}

		template <typename Archive>
		void load(Archive& archive) {
			archive(position, size);
		}

		//! Whether @p box contains @p point.
		//! @note Lower bounds are inclusive, and upper bounds are exclusive.
		template <typename... Quantities>
		constexpr bool contains(box<Quantities...> const& box, point<Quantities...> const& point) {
			for (int i = 0; i < n; ++i) {
				if (point[i] < box.position[i] || point[i] >= box.position[i] + box.size[i]) {
					return false;
				}
			}
			return true;
		}

		//! Extends this box, if necessary, to include the given point.
		void extend(point const& point) {
			for (int i = 0; i < n; ++i) {
				if (point[i] < position[i]) {
					size[i] += position[i] - point[i];
					position[i] = point[i];
				} else if (point[i] > position[i] + size[i]) {
					size[i] = point[i] - position[i];
				}
			}
		}
	};

	//! Template deduction guide for variadic vector construction.
	template <typename... Quantities>
	box(vector<Quantities...>, point<Quantities...>) -> box<Quantities...>;

	namespace detail {
		template <typename Alignment>
		void apply_alignment(Alignment alignment) {
			switch (std::get<Index>(alignment)) {
				case axis<Index>::align::near:
					break;
				case axis<Index>::align::mid:
					position[Index] -= size[Index] / 2 - 1;
					break;
				case axis<Index>::align::far:
					position[Index] -= size[Index] - 1;
					break;
			}
		}
	}

	//! Creates a box of size @p size at @p position with alignment @p alignment.
	//! @param position The position of the box relative to its origin.
	//! @param size The size of the box.
	//! @param alignment The alignment of each of the box's axes, which determines where the origin is.
	template <typename... Quantities, typename... Alignment>
	auto aligned_box(point<Quantities...> position, vector<Quantities...> size, Alignment... alignment) {
		auto result = box{position, size};
		(apply_alignment<Alignment>(result), ...);
		return result;
	}

	//! A rectangular, orthogonal volume in this space.
	struct box {
	private:
		static constexpr point aligned_position(point position, vector size, alignment const& alignment) {
			return apply_alignment<0>(position, size, alignment);
		}

		template <int Index>
		static constexpr point apply_alignment(point position, vector size, alignment const& alignment) {
			//! @todo Specialize these adjustments for integers and floats?
			switch (std::get<Index>(alignment)) {
				case axis<Index>::align::near:
					break;
				case axis<Index>::align::mid:
					position[Index] -= size[Index] / 2 - 1;
					break;
				case axis<Index>::align::far:
					position[Index] -= size[Index] - 1;
					break;
			}
			if constexpr (Index == n - 1) {
				return position;
			} else {
				return apply_alignment<Index + 1>(position, size, alignment);
			}
		}
	};
}

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest.h>
#undef near // Defined in minwindef.h (!)
#undef far // Defined in minwindef.h (!)

TEST_CASE("[box] operations") {
	//! @todo Box tests.
}
