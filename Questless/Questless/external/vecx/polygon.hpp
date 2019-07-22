//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "box.hpp"
#include "point.hpp"

#include <vector>

namespace vecx {
	//! A 2D simple closed polygon defined by a sequence of vertices.
	//! @note Behavior is undefined if the given points do not form a simple closed polygon.
	template <typename Quantity>
	struct polygon {
		using scalar_t = Quantity;
		std::vector<point<scalar_t, 2>> vertices{};

		constexpr polygon() = default;
		constexpr polygon(polygon const&) = default;
		constexpr polygon(polygon&&) = default;

		template <typename... Args>
		constexpr polygon(Args&& ... args) : vertices{std::forward<Args>(args)...} {}

		template <typename ThatQuantity>
		constexpr bool operator ==(polygon<ThatQuantity> const& that) const {
			return vertices == that.vertices;
		}

		template <typename ThatQuantity>
		constexpr bool operator !=(polygon<ThatQuantity> const& that) const {
			return vertices != that.vertices;
		}

		//! The smallest box that contains this polygon.
		constexpr auto bounding_box() const {
			//! @todo Check that the edge cases are correct here. Calling .contains(v) on the result of this function should work for all v in polygon.
			box result{vertices.front(), vector<scalar_t, 2>::zero()};
			for (auto it = vertices.begin() + 1; it != vertices.end(); ++it) {
				result.extend(*it);
			}
			return result;
		}

		//! Whether this polygon contains @p point.
		constexpr bool contains(point<scalar_t, 2> const& point) const {
			//! @todo Rigorously define inclusive vs. exclusive polygon-point containment.

			// First check if the point is outside the bounding box.
			if (!polygon.bounding_box().contains(point)) { return false; }

			// Now check how many edges a ray from the point to the right intersects.
			// Adapted from https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon.
			bool result = false;
			for (std::size_t i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++) {
				if (bool const between_y_coords = vertices[i].y() > point.y() != vertices[j].y() > point.y()) {
					if (point.x() < (vertices[j].x() - vertices[i].x()) * (point.y() - vertices[i].y()) / (vertices[j].y() - vertices[i].y()) + vertices[i].x()) {
						result = !result;
					}
				}
			}
			return result;
		}
	};

	template <typename T, typename... U>
	polygon(T, U...)->polygon<T>;
}

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest/doctest.h>
#undef near // Defined in minwindef.h (!)
#undef far // Defined in minwindef.h (!)

TEST_CASE("[polygon] operations") {
	//! @todo Test integral spaces.
	using namespace cancel;
	using namespace vecx;
	using namespace vecx::literals;

	using u_t = unit_t<struct u_t_tag>;
	using q_t = quantity<double, u_t>;
	using v_t = vector<q_t, 2>;
	using p_t = point<q_t, 2>;
	using poly_t = polygon<q_t>;
	using box_t = box<q_t, 2>;

	p_t p1{q_t{0.0}, q_t{0.0}};
	p_t p2{q_t{1.0}, q_t{0.0}};
	p_t p3{q_t{0.0}, q_t{1.0}};
	poly_t poly{p1, p2, p3};

	SUBCASE("bounding box") {
		box_t const bounding_box
		{p_t{q_t{0.0}, q_t{1.0}}
		, v_t{q_t{1.0}, q_t{1.0}}
		};
		CHECK(poly.bounding_box() == bounding_box);
	}
}
