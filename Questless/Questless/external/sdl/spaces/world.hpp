//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "units/box.hpp"
#include "units/point.hpp"
#include "units/vector.hpp"

namespace units::world {
	using distance = meta::quantity<double, meta::unit_t<struct distance_tag>>;

	namespace literals {
		constexpr auto operator "" _world_distance(long double value) { return distance{static_cast<double>(value)}; }
	}
	using namespace literals;

	using vector = units::vector<distance, 2>;
	using point = units::point<distance, 2>;

	constexpr auto& x(vector& v) { return v[0]; }
	constexpr auto const& x(vector const& v) { return v[0]; }

	constexpr auto& y(vector& v) { return v[1]; }
	constexpr auto const& y(vector const& v) { return v[1]; }

	constexpr auto& x(point& v) { return v[0]; }
	constexpr auto const& x(point const& v) { return v[0]; }

	constexpr auto& y(point& v) { return v[1]; }
	constexpr auto const& y(point const& v) { return v[1]; }

	// Timing and Rates

	using seconds = meta::quantity<double, meta::unit_t<struct world_seconds_tag>>;

	using radians_per_sec = meta::quotient_t<radians, seconds>;

	using speed = meta::quotient_t<distance, seconds>;
	using velocity = decltype(vector{} / seconds{});
	using acceleration = decltype(velocity{} / seconds{});

	using scale_velocity = meta::inverse_t<seconds>;

	// Box Types and Functions

	using box = units::box<distance, 2>;

	using h_align = box::align<0>;
	using v_align = box::align<1>;

	constexpr auto left_align = h_align{h_align::near};
	constexpr auto center_align = h_align{h_align::mid};
	constexpr auto right_align = h_align{h_align::far};

	constexpr auto top_align = v_align{v_align::near};
	constexpr auto middle_align = v_align{v_align::mid};
	constexpr auto bottom_align = v_align{v_align::far};

	constexpr auto& width(box& box) { return x(box.size); }
	constexpr auto width(box const& box) { return x(box.size); }

	constexpr auto& height(box& box) { return y(box.size); }
	constexpr auto height(box const& box) { return y(box.size); }

	constexpr auto& left(box& box) { return x(box.position); }
	constexpr auto left(box const& box) { return x(box.position); }

	constexpr auto& top(box& box) { return y(box.position); }
	constexpr auto top(box const& box) { return y(box.position); }

	constexpr auto right(box const& box) { return x(box.position) + x(box.size); }

	constexpr auto bottom(box const& box) { return y(box.position) + y(box.size); }

	constexpr auto top_left(box const& box) { return box.position; }
	constexpr auto top_right(box const& box) { return point{x(box.position) + x(box.size), y(box.position)}; }
	constexpr auto bottom_left(box const& box) { return point{x(box.position), y(box.position) + y(box.size)}; }
	constexpr auto bottom_right(box const& box) { return box.position + box.size; }

	constexpr auto center(box const& box) { return box.position + box.size / 2.0; }

	constexpr auto area(box const& box) { return width(box) * height(box); }
}
