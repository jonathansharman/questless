//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//!
//! Adapted from the <a href="http://www.redblobgames.com/grids/hexagons/">red Blob Games hexagons tutorial</a>.
//!

#pragma once

#include "ui/view_space.hpp"
#include "utility/unreachable.hpp"

#include <gcem.hpp>

#include <algorithm>
#include <exception>
#include <vector>

namespace ql {
	//! A hexagonal space.
	//! @tparam Tag A type tag used to distinguish between hex spaces with the same length quantity type.
	//! @tparam LengthQuantity A quantity type representing a distance in this hex space.
	template <typename Tag, typename LengthQuantity>
	struct hex_space {
		//! A distance in this space.
		using length_t = LengthQuantity;

		static_assert(std::is_integral_v<length_t::rep>, "The length quantity of a hex space must be integral.");

		enum class direction : int { dr = 0, d, dl, ul, u, ur };

		//! The shortest distance between the two given directions.
		static auto distance(direction d1, direction d2) -> int {
			int diff = static_cast<int>(d1) - static_cast<int>(d2);
			return std::min((diff + 6) % 6, (-diff + 6) % 6);
		}

		//! Hexagonal vector type.
		struct vector {
			length_t q;
			length_t r;

			constexpr length_t s() const {
				return -q - r;
			}

			//! Unit vector in the given direction.
			constexpr static auto unit(direction direction) -> vector {
				switch (direction) {
					default:
						[[fallthrough]]; // Impossible case.
					case direction::dr:
						return vector{1, 0};
					case direction::d:
						return vector{0, 1};
					case direction::dl:
						return vector{-1, 1};
					case direction::ul:
						return vector{-1, 0};
					case direction::u:
						return vector{0, -1};
					case direction::ur:
						return vector{1, -1};
				};
			}

			constexpr explicit vector() : q{0}, r{0} {}
			constexpr explicit vector(length_t q, length_t r) : q{q}, r{r} {}

			constexpr explicit vector(float q, float r, float s) {
				auto round = [this](float v) { return static_cast<typename length_t::rep>(v + 0.5); };
				if (gcem::abs(q - round(q)) > gcem::abs(r - round(r)) && gcem::abs(q - round(q)) > gcem::abs(s - round(s))) {
					this->q = length_t{-round(r) - round(s)};
					this->r = length_t{round(r)};
				} else {
					this->q = length_t{round(q)};
					this->r = length_t{gcem::abs(r - round(r)) > gcem::abs(s - round(s)) ? -round(q) - round(s) : round(r)};
				}
			}

			auto operator<=>(vector const& other) const = default;

			friend constexpr auto operator+(vector v1, vector v2) {
				return vector{v1.q + v2.q, v1.r + v2.r};
			}

			constexpr auto operator-() {
				return vector{-q, -r};
			}

			friend constexpr auto operator-(vector const& v1, vector const& v2) {
				return vector{v1.q - v2.q, v1.r - v2.r};
			}

			friend constexpr auto operator*(vector const& v, int k) {
				return vector{k * v.q, k * v.r};
			}
			friend constexpr auto operator*(int k, vector const& v) {
				return vector{k * v.q, k * v.r};
			}

			friend constexpr auto operator*(vector const& v, length_t k) {
				return vector{k * v.q, k * v.r};
			}
			friend constexpr auto operator*(length_t k, vector v) {
				return vector{k * v.q, k * v.r};
			}

			friend constexpr auto operator/(vector v, int k) {
				return vector{v.q / k, v.r / k};
			}
			friend constexpr auto operator/(vector v, length_t k) {
				return typename hex_space<Tag, cancel::unitless<length_t::rep>>::vector{v.q / k, v.r / k};
			}

			constexpr auto length() const {
				return static_cast<length_t>((gcem::abs(q) + gcem::abs(r) + gcem::abs(s())) / 2);
			}

			//! The unit vector nearest this vector. This vector must be non-zero.
			constexpr auto unit() const {
				auto const l = static_cast<length_t>(length().value);
				assert(l != length_t(0) && "Unit vector of a zero-length vector is undefined.");
				return *this / l;
			}

			//! @todo Add non-zero precondition to direction when available.

			//! The nearest direction this vector points towards. This vector must be non-zero.
			constexpr auto direction() const -> direction {
				auto const u = unit();
				switch (u.q.value) {
					case -1:
						switch (u.r.value) {
							case 0:
								return direction::ul;
							case 1:
								return direction::dl;
							default:
								break;
						}
					case 0:
						switch (u.r.value) {
							case -1:
								return direction::u;
							case 1:
								return direction::d;
							default:
								break;
						}
					case 1:
						switch (u.r.value) {
							case -1:
								return direction::ur;
							case 0:
								return direction::dr;
							default:
								break;
						}
					default:
						break;
				}
				UNREACHABLE;
			}

			//! Simple hash function for hex vectors.
			friend constexpr auto hash_value(vector const& v) -> std::size_t {
				return 31 * v.q.value + v.r.value;
			}
		};

		//! Hexagonal point type.
		struct point {
			length_t q;
			length_t r;

			constexpr auto s() const -> length_t {
				return -q - r;
			}

			constexpr explicit point() : q{0}, r{0} {}
			constexpr explicit point(length_t q, length_t r) : q{q}, r{r} {}
			constexpr explicit point(length_t q, length_t r, length_t s) : q{q}, r{r} {}

			constexpr explicit point(float q, float r, float s) {
				auto round = [this](float v) { return static_cast<typename length_t::rep>(v + 0.5f); };
				if (gcem::abs(q - round(q)) > gcem::abs(r - round(r)) && gcem::abs(q - round(q)) > gcem::abs(s - round(s))) {
					this->q = length_t{-round(r) - round(s)};
					this->r = length_t{round(r)};
				} else {
					this->q = length_t{round(q)};
					this->r = length_t{gcem::abs(r - round(r)) > gcem::abs(s - round(s)) ? -round(q) - round(s) : round(r)};
				}
			}

			auto operator<=>(point const& other) const = default;

			friend constexpr auto operator+(point const& p, vector const& v) {
				return point{p.q + v.q, p.r + v.r};
			}
			friend constexpr auto operator+(vector v, point const& p) {
				return point{v.q + p.q, v.r + p.r};
			}

			friend constexpr auto operator-(point const& p1, point const& p2) {
				return vector{p1.q - p2.q, p1.r - p2.r};
			}
			friend constexpr auto operator-(point const& p, vector v) {
				return point{p.q - v.q, p.r - v.r};
			}

			auto line_to(point dest) const {
				//! @todo Make this lazy?

				int n = (dest - *this).length().value;
				std::vector<point> result;
				result.reserve(n + 1);
				float step = 1.0f / std::max(n, 1);
				for (int i = 0; i <= n; i++) {
					result.push_back(lerp(dest, step * i));
				}
				return result;
			}

			constexpr auto lerp(point dest, float t) const {
				return point{//
					this->q.value + (dest.q - this->q).value * t,
					this->r.value + (dest.r - this->r).value * t,
					this->s().value + (dest.s() - this->s()).value * t};
			}

			constexpr auto lerp(vector heading, float t) const {
				return point{//
					this->q.value + heading.q.value * t,
					this->r.value + heading.r.value * t,
					this->s().value + heading.s().value * t};
			}

			//! Neighboring point in the given direction.
			constexpr auto neighbor(direction direction) const -> point {
				switch (direction) {
					case direction::dr:
						return point{q + length_t{1}, r + length_t{0}};
					case direction::d:
						return point{q + length_t{0}, r + length_t{1}};
					case direction::dl:
						return point{q - length_t{1}, r + length_t{1}};
					case direction::ul:
						return point{q - length_t{1}, r + length_t{0}};
					case direction::u:
						return point{q + length_t{0}, r - length_t{1}};
					case direction::ur:
						return point{q + length_t{1}, r - length_t{1}};
					default:
						UNREACHABLE;
				};
			}

			//! Simple hash function for hex points.
			friend constexpr auto hash_value(point const& p) -> std::size_t {
				return 31 * p.q.value + p.r.value;
			}
		};
	};

	struct hex_orientation {
		// Forward matrix, used to go from hex coords to world coords.
		//  [[f0 f1]
		//   [f2 f2]]
		float f0;
		float f1;
		float f2;
		float f3;
		// Backwards matrix (inverse of forward matrix), used to go from world coords to hex coords.
		//  [[b0 b1]
		//   [b2 b3]]
		float b0;
		float b1;
		float b2;
		float b3;
		float start_angle;

		constexpr hex_orientation(float f0, float f1, float f2, float f3, float b0, float b1, float b2, float b3, float start_angle)
			: f0{f0}, f1{f1}, f2{f2}, f3{f3}, b0{b0}, b1{b1}, b2{b2}, b3{b3}, start_angle{start_angle} {}

		constexpr hex_orientation(float f0, float f1, float f2, float f3, float start_angle)
			: f0{f0}
			, f1{f1}
			, f2{f2}
			, f3{f3}
			, b0{1.0f / (f0 * f3 - f1 * f2) * f3}
			, b1{1.0f / (f0 * f3 - f1 * f2) * -f1}
			, b2{1.0f / (f0 * f3 - f1 * f2) * -f2}
			, b3{1.0f / (f0 * f3 - f1 * f2) * f0}
			, start_angle{start_angle} {}

		constexpr static auto pointy() {
			return hex_orientation{gcem::sqrt(3.0f), gcem::sqrt(3.0f) / 2.0f, 0.0f, 1.5f, 0.5f};
		}
		constexpr static auto flat() {
			return hex_orientation{1.5f, 0.0f, gcem::sqrt(3.0f) / 2.0f, gcem::sqrt(3.0f), 0.0f};
		}
	};

	struct hex_layout {
		hex_orientation orientation;
		view::vector size;
		view::point origin;

		constexpr hex_layout(hex_orientation orientation, view::vector size, view::point origin)
			: orientation{orientation}, size{std::move(size)}, origin{std::move(origin)} {}

		template <typename HexCoordsType>
		constexpr auto to_view_space(HexCoordsType h) const {
			//! @todo This function should just work for tile_hex::point.
			auto const x = (orientation.f0 * h.q.value + orientation.f1 * h.r.value) * size[0] + origin[0];
			auto const y = (orientation.f2 * h.q.value + orientation.f3 * h.r.value) * size[1] + origin[1];
			return view::point{x, y};
		}

		template <typename HexCoordsType>
		constexpr auto to_hex_coords(view::point p) const {
			//! @todo This function should just work for tile_hex::point.
			auto const x = orientation.b0 * (p[0] - origin[0]) / size[0] + orientation.b1 * (p[1] - origin[1]) / size[1];
			auto const y = orientation.b2 * (p[0] - origin[0]) / size[0] + orientation.b3 * (p[1] - origin[1]) / size[1];
			return HexCoordsType{x, y};
		}

		auto hex_corner_offset(int corner) {
			vecx::radians angle = vecx::circle_rad * (corner + orientation.start_angle) / 6.0;
			return view::point{size[0] * static_cast<float>(cos(angle.value)), size[1] * static_cast<float>(sin(angle.value))};
		}

		template <typename HexCoordsType>
		auto corner_points(HexCoordsType h) {
			//! @todo This function should just work for tile_hex::point.
			std::vector<view::point> corners;
			view::point const center = to_view_space_f(h);
			for (int i = 0; i < 6; i++) {
				view::point offset = hex_corner_offset(i);
				corners.push_back(view::point{center[0] + offset[0], center[1] + offset[1]});
			}
			return corners;
		}
	};
}
