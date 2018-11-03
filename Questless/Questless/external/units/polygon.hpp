//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "box.hpp"
#include "point.hpp"

#include <vector>

namespace units {
	//! A simple closed polygon in this space defined by a sequence of vertices. Only defined for 2D spaces.
	//! @note Behavior is undefined if the given points do not form a simple closed polytope.
	template <typename... Quantities>
	struct polygon {
		std::vector<point<Quantities...>> vertices;

		//! The smallest box that contains this polygon.
		static box bounding_box(polygon const& polygon) {
			static_assert(n == 2, "Use of polygon operations requires a 2D type.");

			//! @todo Check that the edge cases are correct here. Calling .contains(v) on the result of this function should work for all v in polygon.
			auto minmax_x = std::accumulate
				( polygon.begin(), polygon.end(), std::make_pair(std::numeric_limits<scalar>::max(), std::numeric_limits<scalar>::min())
				, [](scalar const& acc, point const& v) { return std::minmax(acc.first, acc.second, v.x()); }
				);
			auto minmax_y = std::accumulate
				( polygon.begin(), polygon.end(), std::make_pair(std::numeric_limits<scalar>::max(), std::numeric_limits<scalar>::min())
				, [](scalar const& acc, point const& v) { return std::minmax(acc.first, acc.second, v.y()); }
				);
			auto max_y = std::accumulate
				( polygon.begin(), polygon.end(), std::numeric_limits<scalar>::min()
				, [](scalar const& acc, point const& v) { return std::max(acc, v.y()); }
				);
			return box{point{min_x, min_y}, vector{max_x - min_x, max_y - min_y}};
		}

		//! Whether @p polygon contains @p point.
		static bool contains(polygon const& polygon, point const& point) {
			static_assert(n == 2, "Use of polygon operations requires a 2D type.");

			//! @todo Rigorously define inclusive vs. exclusive polygon-point containment.

			// First check if the point is outside the bounding box.
			if (!bounding_box(polygon).contains(point)) { return false; }

			// Now check how many edges a ray from the point to the right intersects.
			// Adapted from https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon.
			bool result = false;
			for (std::size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
				bool const between_y_coords = polygon[i].y() > point.y() != polygon[j].y() > point.y();
				bool const cond2 = point.x() < (polygon[j].x() - polygon[i].x()) * (point.y() - polygon[i].y()) / (polygon[j].y() - polygon[i].y()) + polygon[i].x();
				if (between_y_coords && cond2) {
					result = !result;
				}
			}
			return result;
		}
	};
}

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest.h>
#undef near // Defined in minwindef.h (!)
#undef far // Defined in minwindef.h (!)

TEST_CASE("[polygon] operations") {
	//! @todo Test integral spaces.

	struct double_space : units::space<struct double_space_tag, double, 2> {};

	double_space::point v1{0.0, 0.0};
	double_space::point v2{1.0, 0.0};
	double_space::point v3{0.0, 1.0};
	double_space::polygon poly{v1, v2, v3};

	SUBCASE("bounding box")
	{
		double_space::box const bounding_box
			{ double_space::point{0.0, 1.0}
			, double_space::vector{1.0, 1.0}
			};
		CHECK(poly.bounding_box() == bounding_box);
	}
}
