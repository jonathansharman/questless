//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "point.hpp"

#include <limits>
#include <type_traits>

namespace units {
	//! A sphere in this space. A circle in 2D spaces, a sphere in 3D spaces, etc.
	template <typename... Quantities>
	struct sphere {
		//! The type of this sphere type's radius.
		using radius_t = void;

		//! The number of dimensions of this sphere type.
		static constexpr std::size_t n = sizeof...(Quantities);

		//! The center point of this sphere.
		point<Quantities...> center;

		//! The radius of this sphere.
		scalar radius;

		template <typename Archive>
		void save(Archive& archive) const {
			archive(center, radius);
		}

		template <typename Archive>
		void load(Archive& archive) {
			archive(center, radius);
		}

		//! Whether this sphere contains @p point. Points on the border are included.
		constexpr bool contains_inclusive(point<Quantities...> const& point) const {
			return (center - point).length_squared() <= radius * radius;
		}

		//! Whether this sphere contains @p point. Points on the border are excluded.
		constexpr bool contains_exclusive(point<Quantities...> const& point) const {
			return (center - point).length_squared() < radius * radius;
		}

		//! Extends this sphere, if necessary, to contain @p point inclusively.
		template <typename... Quantities>
		constexpr void extend_inclusive(point const& point) const {
			if (!contains_inclusive(point)) {
				radius = (center - point).length();
			}
		}

		//! Extends this sphere, if necessary, to contain @p point exclusively.
		template <typename... Quantities>
		constexpr void extend_exclusive(point const& point) const {
			if (!contains_exclusive(point)) {
				radius = (center - point).length();
			}
		}

		//! Extends this sphere, if necessary, to contain @p point inclusively.
		template <typename... Quantities>
		constexpr void extend_inclusive(point const& point) const {
			if (!contains_inclusive(point)) {
				radius = (center - point).length();
				if constexpr (std::is_integral_v<radius_t>) {
					++radius;
				} else {
					// Set radius to the next largest representable value of its type.
					radius = radius_t(std::nextafter(radius.value, std::numeric_limits<radius_t>::max()));
				}
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

//! @todo sphere tests
