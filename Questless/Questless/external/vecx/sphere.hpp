//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "point.hpp"

#include <limits>
#include <type_traits>

namespace vecx {
	//! A sphere in this space. A circle in 2D spaces, a sphere in 3D spaces, etc.
	template <typename Quantity, std::size_t N>
	struct sphere {
		//! The type of scalars in this sphere type.
		using scalar_t = Quantity;

		//! The number of dimensions of this sphere type.
		static constexpr std::size_t n = N;

		//! The center point of this sphere.
		point<scalar_t, n> center;

		//! The radius of this sphere.
		scalar_t radius;

		template <typename ThatQuantity>
		constexpr auto operator ==(sphere<ThatQuantity, n> const& that) const -> bool {
			return center == that.center && radius == that.radius;
		}

		//! Whether this sphere contains @p point. Points on the border are included.
		constexpr auto contains_inclusive(point<scalar_t, n> const& point) const -> bool {
			return (center - point).length_squared() <= radius * radius;
		}

		//! Whether this sphere contains @p point. Points on the border are excluded.
		constexpr auto contains_exclusive(point<scalar_t, n> const& point) const -> bool {
			return (center - point).length_squared() < radius * radius;
		}

		//! Extends this sphere, if necessary, to contain @p point, exclusively.
		constexpr auto extend_exclusive(point<scalar_t, n> const& point) const -> void {
			if (!contains_exclusive(point)) {
				radius = (center - point).length();
			}
		}

		//! Extends this sphere, if necessary, to contain @p point, inclusively.
		constexpr auto extend_inclusive(point<scalar_t, n> const& point) const -> void {
			if (!contains_inclusive(point)) {
				radius = (center - point).length();
				if constexpr (std::is_integral_v<scalar_t>) {
					++radius;
				} else {
					// Set radius to the next largest representable value of its type.
					radius = scalar_t(std::nextafter(radius.value, std::numeric_limits<scalar_t::rep>::max()));
				}
			}
		}
	};
}

#include "doctest_wrapper/test.hpp"

//! @todo sphere tests
