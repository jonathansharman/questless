/**
* @file    HexUtil.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Hexagonal coordinate type, adapted from http://www.redblobgames.com/grids/hexagons/.
*/

#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include <exception>

#include "GameVector.h"
#include "GamePoint.h"
#include "GameRadians.h"
#include "constexpr-math.h"

namespace units
{
	template <typename Tag>
	struct HexCoords
	{
		enum class Direction : int { one = 1, two, three, four, five, six };

		int q;
		int r;
		int s;

		/// @return The shortest distance between the two given directions.
		static int distance(Direction d1, Direction d2)
		{
			int diff = static_cast<int>(d1) - static_cast<int>(d2);
			return std::min((diff + 6) % 6, (-diff + 6) % 6);
		}

		constexpr explicit HexCoords() : q{0}, r{0}, s{0} {}
		constexpr explicit HexCoords(int q, int r) : q{q}, r{r}, s{-q - r} {}
		constexpr explicit HexCoords(int q, int r, int s) : q{q}, r{r}, s{s} {}
		explicit HexCoords(double q, double r)
			: HexCoords{q, r, -q - r}
		{}
		explicit HexCoords(double q, double r, double s)
		{
			this->q = lround(q);
			this->r = lround(r);
			this->s = lround(s);
			double q_diff = abs(q - this->q);
			double r_diff = abs(r - this->r);
			double s_diff = abs(s - this->s);
			if (q_diff > r_diff && q_diff > s_diff) {
				this->q = -this->r - this->s;
			} else {
				if (r_diff > s_diff) {
					this->r = -this->q - this->s;
				} else {
					this->s = -this->q - this->r;
				}
			}
		}

		friend constexpr HexCoords operator +(HexCoords h1, HexCoords h2) { return HexCoords{h1.q + h2.q, h1.r + h2.r, h1.s + h2.s}; }
		friend constexpr HexCoords operator -(HexCoords h1, HexCoords h2) { return HexCoords{h1.q - h2.q, h1.r - h2.r, h1.s - h2.s}; }

		friend constexpr HexCoords operator *(HexCoords h, int k) { return HexCoords{k * h.q, k * h.r, k * h.s}; }
		friend constexpr HexCoords operator *(int k, HexCoords h) { return HexCoords{k * h.q, k * h.r, k * h.s}; }

		friend constexpr HexCoords operator *(HexCoords h, double k) { return HexCoords{k * h.q, k * h.r, k * h.s}; }
		friend constexpr HexCoords operator *(double k, HexCoords h) { return HexCoords{k * h.q, k * h.r, k * h.s}; }

		friend constexpr HexCoords operator /(HexCoords h, int k) { return HexCoords{h.q / k, h.r / k, h.s / k}; }
		friend constexpr HexCoords operator /(HexCoords h, double k) { return HexCoords{h.q / k, h.r / k, h.s / k}; }

		friend constexpr bool operator ==(HexCoords h1, HexCoords h2) { return h1.q == h2.q && h1.r == h2.r && h1.s == h2.s; }
		friend constexpr bool operator !=(HexCoords h1, HexCoords h2) { return h1.q != h2.q || h1.r != h2.r || h1.s != h2.s; }

		/// Arbitrary less-than function so that HexCoords are comparable.
		friend constexpr bool operator <(HexCoords h1, HexCoords h2) { return h1.q < h2.q || (h1.q == h2.q && h1.r < h2.r); }

		constexpr int length() const { return static_cast<int>((abs(q) + abs(r) + abs(s)) / 2); }

		constexpr int distance_to(HexCoords other) const { return (*this - other).length(); }

		std::vector<HexCoords> line_to(HexCoords dest) const
		{
			int n = distance_to(dest);
			std::vector<HexCoords> results = {};
			double step = 1.0 / std::max(n, 1);
			for (int i = 0; i <= n; i++) {
				results.push_back(lerp(dest, step * i));
			}
			return results;
		}

		constexpr HexCoords lerp(HexCoords dest, double t) const
		{
			return HexCoords{this->q + (dest.q - this->q) * t, this->r + (dest.r - this->r) * t, this->s + (dest.s - this->s) * t};
		}

		/// @todo Make offset and neighbor constexpr when constexpr-if becomes available.

		static HexCoords offset(Direction direction)
		{
			switch (direction) {
				case Direction::one:   return HexCoords{1, 0};
				case Direction::two:   return HexCoords{0, 1};
				case Direction::three: return HexCoords{-1, 1};
				case Direction::four:  return HexCoords{-1, 0};
				case Direction::five:  return HexCoords{0, -1};
				default:               return HexCoords{1, -1};
			};
		}

		constexpr HexCoords neighbor(Direction direction) const { return *this + offset(direction); }
	};

	struct OffsetCoords
	{
		int col;
		int row;
		OffsetCoords(int col, int row) : col{col}, row{row} {}
	};

	struct Orientation
	{
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

		constexpr Orientation(double f0, double f1, double f2, double f3, double b0, double b1, double b2, double b3, double start_angle)
			: f0{f0}, f1{f1}, f2{f2}, f3{f3}, b0{b0}, b1{b1}, b2{b2}, b3{b3}, start_angle{start_angle}
		{}

		constexpr Orientation(double f0, double f1, double f2, double f3, double start_angle)
			: f0{f0}, f1{f1}, f2{f2}, f3{f3}
			, b0{1.0 / (f0 * f3 - f1 * f2) * f3}
			, b1{1.0 / (f0 * f3 - f1 * f2) * -f1}
			, b2{1.0 / (f0 * f3 - f1 * f2) * -f2}
			, b3{1.0 / (f0 * f3 - f1 * f2) * f0}
			, start_angle{start_angle}
		{}
	};

	constexpr Orientation orientation_pointy{constexpr_math::sqrt(3.0), constexpr_math::sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, 0.5};
	constexpr Orientation orientation_flat{3.0 / 2.0, 0.0, constexpr_math::sqrt(3.0) / 2.0, constexpr_math::sqrt(3.0), 0.0};

	struct Layout
	{
		Orientation orientation;
		units::GameVector size;
		units::GamePoint origin;

		static constexpr Layout dflt() { return Layout{orientation_flat, units::GameVector{29.0, 35.5 / constexpr_math::sqrt(3.0)}, units::GamePoint{0, 0}}; }

		constexpr Layout(Orientation orientation, units::GameVector size, units::GamePoint origin)
			: orientation{orientation}, size{std::move(size)}, origin{std::move(origin)}
		{}

		template <typename HexCoordsType>
		constexpr units::GamePoint to_world(HexCoordsType h) const /// @todo This function should just work for RegionTileCoords.
		{
			return units::GamePoint
				{ ((orientation.f0 * h.q + orientation.f1 * h.r) * size.x + origin.x)
				, ((orientation.f2 * h.q + orientation.f3 * h.r) * size.y + origin.y)
				};
		}

		template <typename HexCoordsType>
		constexpr HexCoordsType to_hex_coords(units::GamePoint p) const /// @todo This function should just work for RegionTileCoords.
		{
			return HexCoordsType
				{ orientation.b0 * (p.x - origin.x) / size.x + orientation.b1 * (p.y - origin.y) / size.y
				, orientation.b2 * (p.x - origin.x) / size.x + orientation.b3 * (p.y - origin.y) / size.y
				};
		}

		units::GamePoint hex_corner_offset(int corner)
		{
			units::GameRadians angle = units::GameRadians::circle() * (corner + orientation.start_angle) / 6.0;
			return units::GamePoint{size.x * cos(angle.count()), size.y * sin(angle.count())};
		}

		template <typename HexCoordsType>
		std::vector<units::GamePoint> corner_points(HexCoordsType h) /// @todo This function should just work for RegionTileCoords.
		{
			std::vector<units::GamePoint> corners = {};
			units::GamePoint center = to_world_f(h);
			for (int i = 0; i < 6; i++) {
				units::GamePoint offset = hex_corner_offset(i);
				corners.push_back(units::GamePoint{center.x + offset.x, center.y + offset.y});
			}
			return corners;
		}
	};

	template <typename HexCoordsType>
	OffsetCoords q_offset_from_cube(int offset, HexCoordsType h)
	{
		int col = h.q;
		int row = h.r + int((h.q + offset * (h.q & 1)) / 2);
		return OffsetCoords{col, row};
	}

	template <typename HexCoordsType>
	HexCoordsType q_offset_to_cube(int offset, OffsetCoords h)
	{
		int q = h.col;
		int r = h.row - int((h.col + offset * (h.col & 1)) / 2);
		int s = -q - r;
		return HexCoords<Tag>{q, r, s};
	}

	template <typename HexCoordsType>
	OffsetCoords r_offset_from_cube(int offset, HexCoordsType h)
	{
		int col = h.q + int((h.r + offset * (h.r & 1)) / 2);
		int row = h.r;
		return OffsetCoords{col, row};
	}

	template <typename HexCoordsType>
	HexCoordsType r_offset_to_cube(int offset, OffsetCoords h)
	{
		int q = h.col - int((h.row + offset * (h.row & 1)) / 2);
		int r = h.row;
		int s = -q - r;
		return HexCoordsType{q, r, s};
	}
}

// Specialize std::hash.
namespace std
{
	template <typename Tag>
	struct hash<units::HexCoords<Tag>>
	{
		size_t operator()(const units::HexCoords<Tag>& coords) const
		{
			return 31 * coords.q + coords.r;
		}
	};
}

/// @todo Move tests to proper location.
/*

// Tests

const int even = 1;
const int odd = -1;

void complain(const string& name)
{
	cerr << "FAIL " << name << endl;
}

void equal_hex(const string& name, HexCoords a, HexCoords b)
{
	if (!(a.q == b.q && a.s == b.s && a.r == b.r)) {
		complain(name);
	}
}

void equal_offsetcoord(const char* name, OffsetCoords a, OffsetCoords b)
{
	if (!(a.col == b.col && a.row == b.row)) {
		complain(name);
	}
}

void equal_int(const string& name, int a, int b)
{
	if (a != b) {
		complain(name);
	}
}

void equal_hex_array(const string& name, vector<HexCoords> a, vector<HexCoords> b)
{
	equal_int(name, a.size(), b.size());
	for (int i = 0; i < a.size(); i++) {
		equal_hex(name, a[i], b[i]);
	}
}

void test_hex_arithmetic()
{
	equal_hex("hex_add", HexCoords{4, -10, 6}, HexCoords{1, -3, 2} +HexCoords{3, -7, 4});
	equal_hex("hex_subtract", HexCoords{-2, 4, -2}, HexCoords{1, -3, 2} -HexCoords{3, -7, 4});
}

void test_hex_neighbor()
{
	equal_hex("hex_neighbor", HexCoords{1, -3, 2}, HexCoords{1, -2, 1}.neighbor(HexCoords::Direction::three));
}

void test_hex_diagonal()
{
	equal_hex("hex_diagonal", HexCoords{-1, -1, 2}, HexCoords{1, -2, 1}.diagonal_neighbor(HexCoords::Direction::four));
}

void test_hex_distance()
{
	equal_int("hex_distance", 7, HexCoords{3, -7, 4}.distance_to(HexCoords{0, 0, 0}));
}

void test_hex_round()
{
	HexCoords a{0, 0, 0};
	HexCoords b{1, -1, 0};
	HexCoords c{0, -1, 1};
	equal_hex("hex_round 1", HexCoords(5, -10, 5), HexCoords{0, 0, 0}.lerp(HexCoords{10, -20, 10}, 0.5));
	equal_hex("hex_round 2", a, a.lerp(b, 0.499));
	equal_hex("hex_round 3", b, a.lerp(b, 0.501));
	equal_hex("hex_round 4", a, HexCoords{a.q * 0.4 + b.q * 0.3 + c.q * 0.3, a.r * 0.4 + b.r * 0.3 + c.r * 0.3, a.s * 0.4 + b.s * 0.3 + c.s * 0.3});
	equal_hex("hex_round 5", c, HexCoords{a.q * 0.3 + b.q * 0.3 + c.q * 0.4, a.r * 0.3 + b.r * 0.3 + c.r * 0.4, a.s * 0.3 + b.s * 0.3 + c.s * 0.4});
}

void test_hex_linedraw()
{
	equal_hex_array("hex_linedraw", {HexCoords(0, 0, 0), HexCoords(0, -1, 1), HexCoords(0, -2, 2), HexCoords(1, -3, 2), HexCoords(1, -4, 3), HexCoords(1, -5, 4)}, HexCoords{0, 0, 0}.line_to(HexCoords{1, -5, 4}));
}

void test_layout()
{
	HexCoords h{3, 4, -7};
	Layout flat{orientation_flat, units::GamePoint{10, 15}, units::GamePoint{35, 71}};
	equal_hex("layout", h, flat.to_hex_coords(flat.to_world(h)));
	Layout pointy = Layout(orientation_pointy, units::GamePoint{10, 15}, units::GamePoint{35, 71});
	equal_hex("layout", h, pointy.to_hex_coords(pointy.to_world(h)));
}

void test_conversion_roundtrip()
{
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


void test_offset_from_cube()
{
	equal_offsetcoord("offset_from_cube even-q", OffsetCoords{1, 3}, q_offset_from_cube(even, HexCoords{1, 2, -3}));
	equal_offsetcoord("offset_from_cube odd-q", OffsetCoords(1, 2), q_offset_from_cube(odd, HexCoords{1, 2, -3}));
}

void test_offset_to_cube()
{
	equal_hex("offset_to_cube even-", HexCoords{1, 2, -3}, q_offset_to_cube(even, OffsetCoords{1, 3}));
	equal_hex("offset_to_cube odd-q", HexCoords{1, 2, -3}, q_offset_to_cube(odd, OffsetCoords{1, 2}));
}

void test_all()
{
	test_hex_arithmetic();
	test_hex_neighbor();
	test_hex_diagonal();
	test_hex_distance();
	test_hex_round();
	test_hex_linedraw();
	test_layout();
	test_conversion_roundtrip();
	test_offset_from_cube();
	test_offset_to_cube();
}

int main()
{
	test_all();
}

/**/
