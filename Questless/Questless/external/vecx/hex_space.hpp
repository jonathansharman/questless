//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//!
//! Adapted from the <a href="http://www.redblobgames.com/grids/hexagons/">red Blob Games hexagons tutorial</a>.
//!

#pragma once

#include "world/world.hpp"

#include "cancel/quantity.hpp"

#include <gcem.hpp>

#include <algorithm>
#include <cassert> //! @todo Remove when contracts available.
#include <exception>
#include <vector>

namespace vecx {
	//! A hexagonal space.
	//! @tparam LengthQuantity A quantity type representing a distance in this hex space.
	template <typename Tag, typename LengthQuantity>
	struct hex_space {
		//! A distance in this space.
		using length_t = LengthQuantity;

		static_assert(std::is_integral_v<length_t::rep>, "The length quantity of a hex space must be integral.");

		enum class direction : int { one = 1, two, three, four, five, six };

		//! The shortest distance between the two given directions.
		static int distance(direction d1, direction d2) {
			int diff = static_cast<int>(d1) - static_cast<int>(d2);
			return std::min((diff + 6) % 6, (-diff + 6) % 6);
		}

		//! Hexagonal vector type.
		struct vector {
			length_t q;
			length_t r;
			length_t s;

			//! Unit vector in the given direction.
			constexpr static vector unit(direction direction) {
				switch (direction) {
					default: [[fallthrough]]; // Impossible case.
					case direction::one:   return vector{1, 0};
					case direction::two:   return vector{0, 1};
					case direction::three: return vector{-1, 1};
					case direction::four:  return vector{-1, 0};
					case direction::five:  return vector{ 0, -1};
					case direction::six:   return vector{ 1, -1};
				};
			}

			constexpr explicit vector() : q{0}, r{0}, s{0} {}
			constexpr explicit vector(length_t q, length_t r) : q{q}, r{r}, s{-q - r} {}
			constexpr explicit vector(length_t q, length_t r, length_t s) : q{q}, r{r}, s{s} {}
			constexpr explicit vector(double q, double r)
				: vector{q, r, -q - r}
			{}

			constexpr explicit vector(double q, double r, double s) {
				auto round = [this](double v) { return static_cast<typename length_t::rep>(v + 0.5); };
				if (gcem::abs(q - round(q)) > gcem::abs(r - round(r)) && gcem::abs(q - round(q)) > gcem::abs(s - round(s))) {
					this->q = length_t{-round(r) - round(s)};
					this->r = length_t{round(r)};
					this->s = length_t{round(s)};
				} else {
					this->q = length_t{round(q)};
					this->r = length_t{gcem::abs(r - round(r)) > gcem::abs(s - round(s))
						? -round(q) - round(s)
						: round(r)
						};
					this->s = length_t{gcem::abs(r - round(r)) > gcem::abs(s - round(s))
						? round(s)
						: -round(q) - round(r)
						};
				}
			}

			friend constexpr vector operator +(vector v1, vector v2) { return vector{v1.q + v2.q, v1.r + v2.r, v1.s + v2.s}; }

			constexpr vector operator -() { return vector{-q, -r, -s}; }

			friend constexpr vector operator -(vector v1, vector v2) { return vector{v1.q - v2.q, v1.r - v2.r, v1.s - v2.s}; }
		
			friend constexpr vector operator *(vector h, int k) { return vector{k * h.q.value, k * h.r.value, k * h.s.value}; }
			friend constexpr vector operator *(int k, vector h) { return vector{k * h.q.value, k * h.r.value, k * h.s.value}; }

			friend constexpr vector operator *(vector h, double k) { return vector{k * h.q.value, k * h.r.value, k * h.s.value}; }
			friend constexpr vector operator *(double k, vector h) { return vector{k * h.q.value, k * h.r.value, k * h.s.value}; }

			friend constexpr vector operator /(vector h, int k) { return vector{h.q.value / k, h.r.value / k, h.s.value / k}; }
			friend constexpr vector operator /(vector h, double k) { return vector{h.q.value / k, h.r.value / k, h.s.value / k}; }

			friend constexpr bool operator ==(vector v1, vector v2) { return v1.q == v2.q && v1.r == v2.r && v1.s == v2.s; }
			friend constexpr bool operator !=(vector v1, vector v2) { return v1.q != v2.q || v1.r != v2.r || v1.s != v2.s; }

			//! Arbitrary less-than function so that HexCoords are comparable.
			friend constexpr bool operator <(vector v1, vector v2) { return v1.q < v2.q || (v1.q == v2.q && v1.r < v2.r); }

			constexpr length_t length() const { return static_cast<length_t>((gcem::abs(q) + gcem::abs(r) + gcem::abs(s)) / 2); }

			//! The unit vector nearest this vector. This vector must be non-zero.
			constexpr vector unit() const {
				auto const l = static_cast<double>(length().value);
				assert(l != 0.0 && "Unit vector of a zero-length vector is undefined.");
				return *this / l;
			}

			//! The nearest direction this vector points towards. This vector must be non-zero.
			constexpr direction direction() const {
				vector const u = unit();
				switch (u.q.value) {
					case -1:
						switch (u.r.value) {
							case  0: return direction::four;
							case  1: return direction::three;
							default: break;
						}
					case  0:
						switch (u.r.value) {
							case -1: return direction::five;
							case  1: return direction::two;
							default: break;
						}
					case  1:
						switch (u.r.value) {
							case -1: return direction::six;
							case  0: return direction::one;
							default: break;
						}
					default:
						break;
				}
				//! @todo Replace with precondition when available, and remove dummy return statement.
				assert(false && "Vector has no direction.");
				return direction::one;
			}

			//! Simple hash function.
			constexpr friend std::size_t hash_value(vector const& v) {
				return 31 * v.q.value + v.r.value;
			}
		};

		//! Hexagonal point type.
		struct point {
			length_t q;
			length_t r;
			length_t s;

			constexpr explicit point() : q{0}, r{0}, s{0} {}
			constexpr explicit point(length_t q, length_t r) : q{q}, r{r}, s{-q - r} {}
			constexpr explicit point(length_t q, length_t r, length_t s) : q{q}, r{r}, s{s} {}
			constexpr explicit point(double q, double r)
				: point{q, r, -q - r}
			{}

			constexpr explicit point(double q, double r, double s) {
				auto round = [this](double v) { return static_cast<typename length_t::rep>(v + 0.5); };
				if (gcem::abs(q - round(q)) > gcem::abs(r - round(r)) && gcem::abs(q - round(q)) > gcem::abs(s - round(s))) {
					this->q = length_t{-round(r) - round(s)};
					this->r = length_t{round(r)};
					this->s = length_t{round(s)};
				} else {
					this->q = length_t{round(q)};
					this->r = length_t{gcem::abs(r - round(r)) > gcem::abs(s - round(s))
						? -round(q) - round(s)
						: round(r)
						};
					this->s = length_t{gcem::abs(r - round(r)) > gcem::abs(s - round(s))
						? round(s)
						: -round(q) - round(r)
						};
				}
			}

			friend constexpr point operator +(point p, vector v) { return point{p.q + v.q, p.r + v.r, p.s + v.s}; }
			friend constexpr point operator +(vector v, point p) { return point{v.q + p.q, v.r + p.r, v.s + p.s}; }

			friend constexpr vector operator -(point h1, point h2) { return vector{h1.q - h2.q, h1.r - h2.r, h1.s - h2.s}; }
			friend constexpr point operator -(point p, vector v) { return point{p.q - v.q, p.r - v.r, p.s - v.s}; }

			friend constexpr bool operator ==(point p1, point p2) { return p1.q == p2.q && p1.r == p2.r && p1.s == p2.s; }
			friend constexpr bool operator !=(point p1, point p2) { return p1.q != p2.q || p1.r != p2.r || p1.s != p2.s; }

			//! Arbitrary less-than function so that HexPoints are comparable.
			friend constexpr bool operator <(point h1, point h2) { return h1.q < h2.q || (h1.q == h2.q && h1.r < h2.r); }

			//! @todo Make this lazy?

			std::vector<point> line_to(point dest) const {
				int n = (dest - *this).length().value;
				std::vector<point> results;
				double step = 1.0 / std::max(n, 1);
				for (int i = 0; i <= n; i++) {
					results.push_back(lerp(dest, step * i));
				}
				return results;
			}

			constexpr point lerp(point dest, double t) const {
				return point{(this->q + (dest.q - this->q)).value * t, (this->r + (dest.r - this->r)).value * t, (this->s + (dest.s - this->s)).value * t};
			}

			constexpr point lerp(vector heading, double t) const {
				return point{this->q + (heading.q - this->q) * t, this->r + (heading.r - this->r) * t, this->s + (heading.s - this->s) * t};
			}

			//! Neighboring point in the given direction.
			constexpr point neighbor(direction direction) const {
				switch (direction) {
					default: [[fallthrough]]; // Impossible case.
					case direction::one:   return point{q + length_t{1}, r + length_t{0}};
					case direction::two:   return point{q + length_t{0}, r + length_t{1}};
					case direction::three: return point{q - length_t{1}, r + length_t{1}};
					case direction::four:  return point{q - length_t{1}, r + length_t{0}};
					case direction::five:  return point{q + length_t{0}, r - length_t{1}};
					case direction::six:   return point{q + length_t{1}, r - length_t{1}};
				};
			}

			//! Simple hash function.
			constexpr friend std::size_t hash_value(point const& p) {
				return 31 * p.q.value + p.r.value;
			}
		};
	};

	struct hex_orientation {
		// Forward matrix, used to go from hex coords to world coords.
		//  [[f0 f1]
		//   [f2 f2]]
		double f0;
		double f1;
		double f2;
		double f3;
		// Backwards matrix (inverse of forward matrix), used to go from world coords to hex coords.
		//  [[b0 b1]
		//   [b2 b3]]
		double b0;
		double b1;
		double b2;
		double b3;
		double start_angle;

		constexpr hex_orientation(double f0, double f1, double f2, double f3, double b0, double b1, double b2, double b3, double start_angle)
			: f0{f0}, f1{f1}, f2{f2}, f3{f3}, b0{b0}, b1{b1}, b2{b2}, b3{b3}, start_angle{start_angle}
		{}

		constexpr hex_orientation(double f0, double f1, double f2, double f3, double start_angle)
			: f0{f0}, f1{f1}, f2{f2}, f3{f3}
			, b0{1.0 / (f0 * f3 - f1 * f2) * f3}
			, b1{1.0 / (f0 * f3 - f1 * f2) * -f1}
			, b2{1.0 / (f0 * f3 - f1 * f2) * -f2}
			, b3{1.0 / (f0 * f3 - f1 * f2) * f0}
			, start_angle{start_angle}
		{}

		constexpr static auto pointy() { return hex_orientation{gcem::sqrt(3.0), gcem::sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, 0.5}; }
		constexpr static auto flat() { return hex_orientation{3.0 / 2.0, 0.0, gcem::sqrt(3.0) / 2.0, gcem::sqrt(3.0), 0.0}; }
	};

	struct hex_layout {
		hex_orientation orientation;
		ql::world::vector size;
		ql::world::point origin;

		constexpr hex_layout(vecx::hex_orientation orientation, ql::world::vector size, ql::world::point origin)
			: orientation{orientation}, size{std::move(size)}, origin{std::move(origin)}
		{}

		template <typename HexCoordsType>
		constexpr ql::world::point to_world(HexCoordsType h) const {
			//! @todo This function should just work for region_tile::point.
			return world::point
				{ ((orientation.f0 * h.q + orientation.f1 * h.r) * x(size) + x(origin))
				, ((orientation.f2 * h.q + orientation.f3 * h.r) * y(size) + y(origin))
				};
		}

		template <typename HexCoordsType>
		constexpr HexCoordsType to_hex_coords(ql::world::point p) const {
			//! @todo This function should just work for region_tile::point.
			return HexCoordsType
				{ orientation.b0 * (x(p) - x(origin)) / x(size) + orientation.b1 * (y(p) - y(origin)) / y(size)
				, orientation.b2 * (x(p) - x(origin)) / x(size) + orientation.b3 * (y(p) - y(origin)) / y(size)
				};
		}

		ql::world::point hex_corner_offset(int corner) {
			radians angle = circle_rad * (corner + orientation.start_angle) / 6.0;
			return ql::world::point{x(size) * cos(angle.value), y(size) * sin(angle.value)};
		}

		template <typename HexCoordsType>
		std::vector<ql::world::point> corner_points(HexCoordsType h) {
			//! @todo This function should just work for region_tile::point.
			std::vector<world::point> corners;
			ql::world::point const center = to_world_f(h);
			for (int i = 0; i < 6; i++) {
				ql::world::point offset = hex_corner_offset(i);
				corners.push_back(world::point{x(center) + x(offset), y(center) + y(offset)});
			}
			return corners;
		}
	};
}

//! @todo Replace these with doctest tests.
/*

// Tests

int const even = 1;
int const odd = -1;

void complain(string_view name) {
	cerr << "FAIL " << name << endl;
}

void equal_hex(string_view name, HexCoords a, HexCoords b) {
	if (!(a.q == b.q && a.s == b.s && a.r == b.r)) {
		complain(name);
	}
}

void equal_offsetcoord(const char* name, OffsetCoords a, OffsetCoords b) {
	if (!(a.col == b.col && a.row == b.row)) {
		complain(name);
	}
}

void equal_int(string_view name, int a, int b) {
	if (a != b) {
		complain(name);
	}
}

void equal_hex_array(string_view name, vector<HexCoords> a, vector<HexCoords> b) {
	equal_int(name, a.size(), b.size());
	for (int i = 0; i < a.size(); i++) {
		equal_hex(name, a[i], b[i]);
	}
}

void test_hex_arithmetic() {
	equal_hex("hex_add", HexCoords{4, -10, 6}, HexCoords{1, -3, 2} +HexCoords{3, -7, 4});
	equal_hex("hex_subtract", HexCoords{-2, 4, -2}, HexCoords{1, -3, 2} -HexCoords{3, -7, 4});
}

void test_hex_neighbor() {
	equal_hex("hex_neighbor", HexCoords{1, -3, 2}, HexCoords{1, -2, 1}.neighbor(HexCoords::direction::three));
}

void test_hex_diagonal() {
	equal_hex("hex_diagonal", HexCoords{-1, -1, 2}, HexCoords{1, -2, 1}.diagonal_neighbor(HexCoords::direction::four));
}

void test_hex_distance() {
	equal_int("hex_distance", 7, HexCoords{3, -7, 4}.distance_to(HexCoords{0, 0, 0}));
}

void test_hex_round() {
	HexCoords a{0, 0, 0};
	HexCoords b{1, -1, 0};
	HexCoords c{0, -1, 1};
	equal_hex("hex_round 1", HexCoords(5, -10, 5), HexCoords{0, 0, 0}.lerp(HexCoords{10, -20, 10}, 0.5));
	equal_hex("hex_round 2", a, a.lerp(b, 0.499));
	equal_hex("hex_round 3", b, a.lerp(b, 0.501));
	equal_hex("hex_round 4", a, HexCoords{a.q * 0.4 + b.q * 0.3 + c.q * 0.3, a.r * 0.4 + b.r * 0.3 + c.r * 0.3, a.s * 0.4 + b.s * 0.3 + c.s * 0.3});
	equal_hex("hex_round 5", c, HexCoords{a.q * 0.3 + b.q * 0.3 + c.q * 0.4, a.r * 0.3 + b.r * 0.3 + c.r * 0.4, a.s * 0.3 + b.s * 0.3 + c.s * 0.4});
}

void test_hex_linedraw() {
	equal_hex_array("hex_linedraw", {HexCoords(0, 0, 0), HexCoords(0, -1, 1), HexCoords(0, -2, 2), HexCoords(1, -3, 2), HexCoords(1, -4, 3), HexCoords(1, -5, 4)}, HexCoords{0, 0, 0}.line_to(HexCoords{1, -5, 4}));
}

void test_layout() {
	HexCoords h{3, 4, -7};
	layout flat{orientation_flat, world::point{10, 15}, world::point{35, 71}};
	equal_hex("layout", h, flat.to_hex_coords(flat.to_world(h)));
	layout pointy = layout(orientation_pointy, world::point{10, 15}, world::point{35, 71});
	equal_hex("layout", h, pointy.to_hex_coords(pointy.to_world(h)));
}

if not main_byid_df.ix[sampleList[i], 'Grain_ID'][j] in rimVsCore

void test_conversion_roundtrip() {
	HexCoords a{3, 4, -7};
	OffsetCoords b{1, -3};
	equal_hex("conversion_roundtrip even-q", a, q_offset_to_cube(even, q_offset_from_cube(even, a)));
	equal_offsetcoord("conversion_roundtrip even-q", b, q_offset_from_cube(even, q_offset_to_cube(even, b)));
	equal_hex("conversion_roundtrip odd-q", a, q_offset_to_cube(odd, q_offset_from_cube(odd, a)));
	equal_offsetcoord("conversion_roundtrip odd-q", b, q_offset_from_cube(odd, q_offset_to_cube(odd, b)));
	equal_hex("conversion_roundtrip even-r", a, r_offset_to_cube(even, r_offset_from_cube(even, a)));
	equal_offsetcoord("conversion_roundtrip even-r", b, r_offset_from_cube(even, r_offset_to_cube(even, b)));
	equal_hex("conversion_roundtrip odd-r", a, r_offset_to_cube(odd, r_offset_from_cube(odd, a)));
	equal_offsetcoord("conversion_roundtrip odd-r", b, r_offset_from_cube(odd, r_offset_to_cube(odd, b)));
}


void test_offset_from_cube() {
	equal_offsetcoord("offset_from_cube even-q", OffsetCoords{1, 3}, q_offset_from_cube(even, HexCoords{1, 2, -3}));
	equal_offsetcoord("offset_from_cube odd-q", OffsetCoords(1, 2), q_offset_from_cube(odd, HexCoords{1, 2, -3}));
}

void test_offset_to_cube() {
	equal_hex("offset_to_cube even-", HexCoords{1, 2, -3}, q_offset_to_cube(even, OffsetCoords{1, 3}));
	equal_hex("offset_to_cube odd-q", HexCoords{1, 2, -3}, q_offset_to_cube(odd, OffsetCoords{1, 2}));
}

/**/