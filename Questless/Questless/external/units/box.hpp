//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "point.hpp"
#include "vector.hpp"

namespace units {
	//! An n-dimensional rectangular volume.
	template <typename Quantity, std::size_t N>
	struct box {
		//! The type of scalars in this box type.
		using scalar_t = Quantity;

		//! The number of dimensions of this box type.
		static constexpr std::size_t n = N;

		//! Alignment along the axis with index @p Axis.
		template <std::size_t Axis>
		struct align {
			static constexpr std::size_t axis = Axis;
			enum { near, mid, far } align;
		};

		//! The position of the minimal corner of this box.
		point<scalar_t, n> position;

		//! The size or extent of this box.
		vector<scalar_t, n> size;

		//! Creates a box of size @p size at @p position with alignment @p alignment.
		//! @param position The position of the box relative to its origin.
		//! @param size The size of the box.
		//! @param aligns A variadic list of alignments to apply to the box.
		template <typename Quantity, std::size_t N, typename... Aligns>
		static constexpr auto aligned(point<Quantity, N> position, vector<Quantity, N> size, Aligns... aligns) {
			auto result = box{position, size};
			auto apply_align = [&result](auto align) {
				using t = decltype(align);
				switch (align.align) {
					case t::near:
						break;
					case t::mid:
						result.position[t::axis] -= size[t::axis] / 2 - 1;
						break;
					case t::far:
						result.position[t::axis] -= size[t::axis] - 1;
						break;
				}
			};
			(apply_align(aligns), ...);
			return result;
		}

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
		void extend(point<scalar_t, n> const& point) {
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

	template <typename Quantity, std::size_t N>
	box(vector<Quantity, N>, point<Quantity, N>) -> box<Quantity, N>;
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
