//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//!
//! Adapted from the <a href="http://www.redblobgames.com/grids/hexagons/">Red Blob Games hexagons tutorial</a>.
//!

#pragma once

#include <vector>
#include <algorithm>
#include <exception>

#include "GameVector.h"
#include "GamePoint.h"
#include "GameRadians.h"
#include "math.h"

namespace units
{
	//! A hexagonal space.
	template <typename Tag>
	struct HexSpace
	{
		enum class Direction : int { one = 1, two, three, four, five, six };

		//! The shortest distance between the two given directions.
		static int distance(Direction d1, Direction d2)
		{
			int diff = static_cast<int>(d1) - static_cast<int>(d2);
			return std::min((diff + 6) % 6, (-diff + 6) % 6);
		}

		//! Hexagonal point type.
		struct Point
		{
			int q;
			int r;
			int s;

			constexpr explicit Point() = default;
			constexpr explicit Point(int q, int r) : q{q}, r{r}, s{-q - r} {}
			constexpr explicit Point(int q, int r, int s) : q{q}, r{r}, s{s} {}
			explicit Point(double q, double r)
				: Point{q, r, -q - r}
			{}

			constexpr explicit Point(double q, double r, double s)
				: q{math::abs(q - math::lround(q)) > math::abs(r - math::lround(r)) && math::abs(q - math::lround(q)) > math::abs(s - math::lround(s))
					? -math::lround(r) - math::lround(s)
					: math::lround(q)
					}
				, r{math::abs(q - math::lround(q)) > math::abs(r - math::lround(r)) && math::abs(q - math::lround(q)) > math::abs(s - math::lround(s))
					? math::lround(r)
					: math::abs(r - math::lround(r)) > math::abs(s - math::lround(s))
						? -math::lround(q) - math::lround(s)
						: math::lround(r)
					}
				, s{math::abs(q - math::lround(q)) > math::abs(r - math::lround(r)) && math::abs(q - math::lround(q)) > math::abs(s - math::lround(s))
					? math::lround(s)
					: math::abs(r - math::lround(r)) > math::abs(s - math::lround(s))
						? math::lround(s)
						: -math::lround(q) - math::lround(r)
					}
			{}

			friend constexpr Vector operator -(Point h1, Point h2) { return Vector{h1.q - h2.q, h1.r - h2.r, h1.s - h2.s}; }

			friend constexpr bool operator ==(Point p1, Point p2) { return p1.q == p2.q && p1.r == p2.r && p1.s == p2.s; }
			friend constexpr bool operator !=(Point p1, Point p2) { return p1.q != p2.q || p1.r != p2.r || p1.s != p2.s; }

			//! Arbitrary less-than function so that HexPoints are comparable.
			friend constexpr bool operator <(Point h1, Point h2) { return h1.q < h2.q || (h1.q == h2.q && h1.r < h2.r); }

			//! @todo Make this lazy?

			std::vector<Point> line_to(Point dest) const
			{
				int n = (dest - *this).length();
				std::vector<Point> results;
				double step = 1.0 / std::max(n, 1);
				for (int i = 0; i <= n; i++) {
					results.push_back(lerp(dest, step * i));
				}
				return results;
			}

			constexpr Point lerp(Point dest, double t) const
			{
				return Point{this->q + (dest.q - this->q) * t, this->r + (dest.r - this->r) * t, this->s + (dest.s - this->s) * t};
			}

			constexpr Point lerp(Vector heading, double t) const
			{
				return Point{this->q + (heading.q - this->q) * t, this->r + (heading.r - this->r) * t, this->s + (heading.s - this->s) * t};
			}

			//! Neighboring point in the given direction.
			constexpr Point neighbor(Direction direction) const
			{
				switch (direction) {
					default: [[fallthrough]]; // Impossible case.
					case Direction::one:   return Point{q + 1, r + 0};
					case Direction::two:   return Point{q + 0, r + 1};
					case Direction::three: return Point{q - 1, r + 1};
					case Direction::four:  return Point{q - 1, r + 0};
					case Direction::five:  return Point{q + 0, r - 1};
					case Direction::six:   return Point{q + 1, r - 1};
				};
			}

			//! Simple hash function.
			constexpr friend std::size_t hash_value(Point const& p)
			{
				return 31 * p.q + p.r;
			}
		};

		//! Hexagonal vector type.
		struct Vector
		{
			int q;
			int r;
			int s;

			//! Unit vector in the given direction.
			constexpr static Vector unit(Direction direction)
			{
				switch (direction) {
					default: [[fallthrough]]; // Impossible case.
					case Direction::one:   return Vector{ 1,  0};
					case Direction::two:   return Vector{ 0,  1};
					case Direction::three: return Vector{-1,  1};
					case Direction::four:  return Vector{-1,  0};
					case Direction::five:  return Vector{ 0, -1};
					case Direction::six:   return Vector{ 1, -1};
				};
			}

			constexpr explicit Vector() = default;
			constexpr explicit Vector(int q, int r) : q{q}, r{r}, s{-q - r} {}
			constexpr explicit Vector(int q, int r, int s) : q{q}, r{r}, s{s} {}
			constexpr explicit Vector(double q, double r)
				: Vector{q, r, -q - r}
			{}

			constexpr explicit Vector(double q, double r, double s)
				: q{math::abs(q - math::lround(q)) > math::abs(r - math::lround(r)) && math::abs(q - math::lround(q)) > math::abs(s - math::lround(s))
					? -math::lround(r) - math::lround(s)
					: math::lround(q)
					}
				, r{math::abs(q - math::lround(q)) > math::abs(r - math::lround(r)) && math::abs(q - math::lround(q)) > math::abs(s - math::lround(s))
					? math::lround(r)
					: math::abs(r - math::lround(r)) > math::abs(s - math::lround(s))
						? -math::lround(q) - math::lround(s)
						: math::lround(r)
					}
				, s{math::abs(q - math::lround(q)) > math::abs(r - math::lround(r)) && math::abs(q - math::lround(q)) > math::abs(s - math::lround(s))
					? math::lround(s)
					: math::abs(r - math::lround(r)) > math::abs(s - math::lround(s))
						? math::lround(s)
						: -math::lround(q) - math::lround(r)
					}
			{}

			friend constexpr Vector operator +(Vector v1, Vector v2) { return Vector{v1.q + v2.q, v1.r + v2.r, v1.s + v2.s}; }
			friend constexpr Point operator +(Point p, Vector v) { return Point{p.q + v.q, p.r + v.r, p.s + v.s}; }
			friend constexpr Point operator +(Vector v, Point p) { return Point{v.q + p.q, v.r + p.r, v.s + p.s}; }

			constexpr Vector operator -() { return Vector{-q, -r, -s}; }

			friend constexpr Vector operator -(Vector v1, Vector v2) { return Vector{v1.q - v2.q, v1.r - v2.r, v1.s - v2.s}; }
			friend constexpr Point operator -(Point p, Vector v) { return Point{p.q - v.q, p.r - v.r, p.s - v.s}; }
		
			friend constexpr Vector operator *(Vector h, int k) { return Vector{k * h.q, k * h.r, k * h.s}; }
			friend constexpr Vector operator *(int k, Vector h) { return Vector{k * h.q, k * h.r, k * h.s}; }

			friend constexpr Vector operator *(Vector h, double k) { return Vector{k * h.q, k * h.r, k * h.s}; }
			friend constexpr Vector operator *(double k, Vector h) { return Vector{k * h.q, k * h.r, k * h.s}; }

			friend constexpr Vector operator /(Vector h, int k) { return Vector{h.q / k, h.r / k, h.s / k}; }
			friend constexpr Vector operator /(Vector h, double k) { return Vector{h.q / k, h.r / k, h.s / k}; }

			friend constexpr bool operator ==(Vector v1, Vector v2) { return v1.q == v2.q && v1.r == v2.r && v1.s == v2.s; }
			friend constexpr bool operator !=(Vector v1, Vector v2) { return v1.q != v2.q || v1.r != v2.r || v1.s != v2.s; }

			//! Arbitrary less-than function so that HexCoords are comparable.
			friend constexpr bool operator <(Vector v1, Vector v2) { return v1.q < v2.q || (v1.q == v2.q && v1.r < v2.r); }

			constexpr int length() const { return static_cast<int>((math::abs(q) + math::abs(r) + math::abs(s)) / 2); }

			//! The unit vector nearest this vector. Throws a @p std::domain_error for the zero vector.
			constexpr Vector unit() const
			{
				double l = length();
				if (l == 0) {
					throw std::domain_error{"Unit vector of a zero-length vector is undefined."};
				}
				return *this / static_cast<double>(l);
			}

			//! The nearest direction this vector points towards. Throws a @p std::domain_error for the zero vector.
			constexpr Direction direction() const
			{
				Vector const u = unit();
				switch (u.q) {
					case -1:
						switch (u.r) {
							case  0: return Direction::four;
							case  1: return Direction::three;
						}
					case  0:
						switch (u.r) {
							case -1: return Direction::five;
							case  1: return Direction::two;
						}
					case  1:
						switch (u.r) {
							case -1: return Direction::six;
							case  0: return Direction::one;
						}
				}
			}

			//! Simple hash function.
			constexpr friend std::size_t hash_value(Vector const& v)
			{
				return 31 * v.q + v.r;
			}
		};
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

	constexpr Orientation orientation_pointy{math::sqrt(3.0), math::sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, 0.5};
	constexpr Orientation orientation_flat{3.0 / 2.0, 0.0, math::sqrt(3.0) / 2.0, math::sqrt(3.0), 0.0};

	struct Layout
	{
		Orientation orientation;
		units::GameVector size;
		units::GamePoint origin;

		static constexpr Layout dflt() { return Layout{orientation_flat, units::GameVector{30.0, 36.0 / math::sqrt(3.0)}, units::GamePoint{0, 0}}; }

		constexpr Layout(Orientation orientation, units::GameVector size, units::GamePoint origin)
			: orientation{orientation}, size{std::move(size)}, origin{std::move(origin)}
		{}

		template <typename HexCoordsType>
		constexpr units::GamePoint to_world(HexCoordsType h) const //! @todo This function should just work for RegionTile::Point.
		{
			return units::GamePoint
				{ ((orientation.f0 * h.q + orientation.f1 * h.r) * size.x + origin.x)
				, ((orientation.f2 * h.q + orientation.f3 * h.r) * size.y + origin.y)
				};
		}

		template <typename HexCoordsType>
		constexpr HexCoordsType to_hex_coords(units::GamePoint p) const //! @todo This function should just work for RegionTile::Point.
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
		std::vector<units::GamePoint> corner_points(HexCoordsType h) //! @todo This function should just work for RegionTile::Point.
		{
			std::vector<units::GamePoint> corners;
			units::GamePoint const center = to_world_f(h);
			for (int i = 0; i < 6; i++) {
				units::GamePoint offset = hex_corner_offset(i);
				corners.push_back(units::GamePoint{center.x + offset.x, center.y + offset.y});
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

void complain(string_view name)
{
	cerr << "FAIL " << name << endl;
}

void equal_hex(string_view name, HexCoords a, HexCoords b)
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

void equal_int(string_view name, int a, int b)
{
	if (a != b) {
		complain(name);
	}
}

void equal_hex_array(string_view name, vector<HexCoords> a, vector<HexCoords> b)
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
