//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "point.hpp"
#include "vector.hpp"

namespace vecx {
	//! An n-dimensional rectangular volume.
	template <typename Quantity, std::size_t N>
	struct box {
		//! The type of scalars in this box type.
		using scalar_t = Quantity;

		//! The number of dimensions of this box type.
		static constexpr std::size_t n = N;

		//! The axis with index @p index.
		template <std::size_t Index>
		struct axis {
			static constexpr std::size_t index = Index;
			enum class align_t { near, mid, far } align;
		};

		//! The position of the minimal corner of this box.
		point<scalar_t, n> position{};

		//! The size or extent of this box.
		vector<scalar_t, n> size{};

		constexpr box() = default;
		constexpr box(box const&) = default;
		constexpr box(box&&) = default;

		//! @param position The position of the minimal corner of this box.
		//! @param size The size or extent of this box.
		constexpr box(point<scalar_t, n> position, vector<scalar_t, n> size)
			: position{position}, size{size}
		{}

		constexpr box& operator =(box const& other) {
			this->position = other.position;
			this->size = other.size;
			return *this;
		}
		constexpr box& operator =(box&& other) {
			this->position = std::move(other.position);
			this->size = std::move(other.size);
			return *this;
		}

		constexpr bool operator ==(box const& that) const {
			return position == that.position && size == that.size;
		};

		constexpr bool operator !=(box const& that) const {
			return position != that.position || size != that.size;
		}

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

		//! The center point of this box.
		constexpr auto center(box const& box) { return box.position + box.size / 2; }

		//! The n-dimensional volume of this box.
		constexpr auto volume() const {
			return size.reduce(scalar_t(1), std::multiplies{});
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
	box(point<Quantity, N>, vector<Quantity, N>)->box<Quantity, N>;
}

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest/doctest.h>
#undef near // Defined in minwindef.h (!)
#undef far // Defined in minwindef.h (!)

TEST_CASE("[box] operations") {
	//! @todo Box tests.
}
