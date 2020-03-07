//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//!
//! Adapted from the <a href="http://www.redblobgames.com/grids/hexagons/">red Blob Games hexagons tutorial</a>.
//!

#pragma once

#include "utility/unreachable.hpp"

#include <vecx/point.hpp>
#include <vecx/vector.hpp>

#include <gcem.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <exception>
#include <vector>

namespace ql {
	enum class hex_direction : int { dr = 0, d, dl, ul, u, ur };

	//! The shortest distance between the two given directions.
	inline auto distance(hex_direction d1, hex_direction d2) -> int {
		int diff = static_cast<int>(d1) - static_cast<int>(d2);
		return std::min((diff + 6) % 6, (-diff + 6) % 6);
	}

	//! Hexagonal vector type.
	template <typename LengthQuantity>
	struct hex_vector {
		using length_t = LengthQuantity;

		static_assert(std::is_integral_v<length_t::rep>, "The length_t quantity of a hex space must be integral.");

		length_t q;
		length_t r;

		constexpr length_t s() const {
			return -q - r;
		}

		//! Unit vector in the given direction.
		constexpr static auto unit(hex_direction direction) -> hex_vector {
			switch (direction) {
				default:
					[[fallthrough]]; // Impossible case.
				case hex_direction::dr:
					return {1, 0};
				case hex_direction::d:
					return {0, 1};
				case hex_direction::dl:
					return {-1, 1};
				case hex_direction::ul:
					return {-1, 0};
				case hex_direction::u:
					return {0, -1};
				case hex_direction::ur:
					return {1, -1};
			};
		}

		constexpr hex_vector() : q{0}, r{0} {}
		constexpr hex_vector(length_t q, length_t r) : q{q}, r{r} {}

		constexpr hex_vector(float q, float r, float s) {
			auto round = [this](float v) { return static_cast<typename length_t::rep>(v + 0.5); };
			if (gcem::abs(q - round(q)) > gcem::abs(r - round(r)) && gcem::abs(q - round(q)) > gcem::abs(s - round(s))) {
				this->q = length_t{-round(r) - round(s)};
				this->r = length_t{round(r)};
			} else {
				this->q = length_t{round(q)};
				this->r = length_t{gcem::abs(r - round(r)) > gcem::abs(s - round(s)) ? -round(q) - round(s) : round(r)};
			}
		}

		auto operator<=>(hex_vector const& other) const = default;

		friend constexpr auto operator+(hex_vector v1, hex_vector v2) {
			return hex_vector{v1.q + v2.q, v1.r + v2.r};
		}

		constexpr auto operator-() {
			return hex_vector{-q, -r};
		}

		friend constexpr auto operator-(hex_vector const& v1, hex_vector const& v2) {
			return hex_vector{v1.q - v2.q, v1.r - v2.r};
		}

		friend constexpr auto operator*(hex_vector const& v, int k) {
			return hex_vector{k * v.q, k * v.r};
		}
		friend constexpr auto operator*(int k, hex_vector const& v) {
			return hex_vector{k * v.q, k * v.r};
		}

		friend constexpr auto operator*(hex_vector const& v, length_t k) {
			return hex_vector{k * v.q, k * v.r};
		}
		friend constexpr auto operator*(length_t k, hex_vector v) {
			return hex_vector{k * v.q, k * v.r};
		}

		friend constexpr auto operator/(hex_vector v, int k) {
			return hex_vector{v.q / k, v.r / k};
		}
		friend constexpr auto operator/(hex_vector v, length_t k) {
			return hex_vector<cancel::unitless<length_t::rep>>{v.q / k, v.r / k};
		}

		constexpr auto length() const -> length_t {
			return (gcem::abs(q) + gcem::abs(r) + gcem::abs(s())) / 2;
		}

		//! The unit vector nearest this vector. This vector must be non-zero.
		constexpr auto unit() const {
			auto const l = length_t();
			assert(l != length_t(0) && "Unit vector of a zero-length_t vector is undefined.");
			return *this / l;
		}

		//! @todo Add non-zero precondition to direction when available.

		//! The nearest direction this vector points towards. This vector must be non-zero.
		constexpr auto direction() const -> hex_direction {
			auto const u = unit();
			switch (u.q.value) {
				case -1:
					switch (u.r.value) {
						case 0:
							return hex_direction::ul;
						case 1:
							return hex_direction::dl;
						default:
							break;
					}
				case 0:
					switch (u.r.value) {
						case -1:
							return hex_direction::u;
						case 1:
							return hex_direction::d;
						default:
							break;
					}
				case 1:
					switch (u.r.value) {
						case -1:
							return hex_direction::ur;
						case 0:
							return hex_direction::dr;
						default:
							break;
					}
				default:
					break;
			}
			UNREACHABLE;
		}

		//! Simple hash function for hex vectors.
		friend constexpr auto hash_value(hex_vector const& v) -> std::size_t {
			return 31 * v.q.value + v.r.value;
		}
	};

	//! Hexagonal point type.
	template <typename LengthQuantity>
	struct hex_point {
		using length_t = LengthQuantity;

		length_t q;
		length_t r;

		constexpr auto s() const -> length_t {
			return -q - r;
		}

		constexpr hex_point() : q{0}, r{0} {}
		constexpr hex_point(length_t q, length_t r) : q{q}, r{r} {}
		constexpr hex_point(length_t q, length_t r, length_t s) : q{q}, r{r} {}

		constexpr explicit hex_point(float q, float r, float s) {
			auto round = [this](float v) { return static_cast<typename length_t::rep>(v + 0.5f); };
			if (gcem::abs(q - round(q)) > gcem::abs(r - round(r)) && gcem::abs(q - round(q)) > gcem::abs(s - round(s))) {
				this->q = length_t{-round(r) - round(s)};
				this->r = length_t{round(r)};
			} else {
				this->q = length_t{round(q)};
				this->r = length_t{gcem::abs(r - round(r)) > gcem::abs(s - round(s)) ? -round(q) - round(s) : round(r)};
			}
		}

		auto operator<=>(hex_point const& other) const = default;

		friend constexpr auto operator+(hex_point const& p, hex_vector<length_t> const& v) -> hex_point {
			return {p.q + v.q, p.r + v.r};
		}
		friend constexpr auto operator+(hex_vector<length_t> v, hex_point const& p) -> hex_point {
			return {v.q + p.q, v.r + p.r};
		}

		friend constexpr auto operator-(hex_point const& p1, hex_point const& p2) -> hex_vector<length_t> {
			return {p1.q - p2.q, p1.r - p2.r};
		}
		friend constexpr auto operator-(hex_point const& p, hex_vector<length_t> v) -> hex_point {
			return {p.q - v.q, p.r - v.r};
		}

		//! Interpolates @p proportion of the way from this point to @p dest.
		constexpr auto lerp(hex_point dest, float proportion) const -> hex_point {
			return hex_point{//
				this->q.value + (dest.q - this->q).value * proportion,
				this->r.value + (dest.r - this->r).value * proportion,
				this->s().value + (dest.s() - this->s()).value * proportion};
		}

		//! Interpolates @p proportion of the way from this point towards @p heading.
		constexpr auto lerp(hex_vector<length_t> heading, float proportion) const {
			return lerp(*this + heading, proportion);
		}

		//! A range of points on the shortest line from this point to @p dest, inclusive.
		auto line_to(hex_point dest) const {
			int n = (dest - *this).length().value + 1;
			float const step = 1.0f / std::max(n, 1);
			return ranges::views::ints(0, n) |
				ranges::views::transform([src = *this, dest, step](int i) { return src.lerp(dest, step * i); });
		}

		//! Neighboring point in the given direction.
		constexpr auto neighbor(hex_direction direction) const -> hex_point {
			switch (direction) {
				case hex_direction::dr:
					return {q + length_t{1}, r + length_t{0}};
				case hex_direction::d:
					return {q + length_t{0}, r + length_t{1}};
				case hex_direction::dl:
					return {q - length_t{1}, r + length_t{1}};
				case hex_direction::ul:
					return {q - length_t{1}, r + length_t{0}};
				case hex_direction::u:
					return {q + length_t{0}, r - length_t{1}};
				case hex_direction::ur:
					return {q + length_t{1}, r - length_t{1}};
				default:
					UNREACHABLE;
			};
		}

		//! Simple hash function for hex points.
		friend constexpr auto hash_value(hex_point const& p) -> std::size_t {
			return 31 * p.q.value + p.r.value;
		}
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

		vecx::radians start_angle;

		constexpr hex_orientation(float f0, float f1, float f2, float f3, float b0, float b1, float b2, float b3, vecx::radians start_angle)
			: f0{f0}, f1{f1}, f2{f2}, f3{f3}, b0{b0}, b1{b1}, b2{b2}, b3{b3}, start_angle{start_angle} {}

		constexpr hex_orientation(float f0, float f1, float f2, float f3, vecx::radians start_angle)
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
			return hex_orientation{gcem::sqrt(3.0f), gcem::sqrt(3.0f) / 2.0f, 0.0f, 1.5f, vecx::radians{0.5}};
		}
		constexpr static auto flat() {
			return hex_orientation{1.5f, 0.0f, gcem::sqrt(3.0f) / 2.0f, gcem::sqrt(3.0f), vecx::radians{0.0}};
		}
	};

	//! Determines how a hexagonal grid maps to and from world space.
	//! @tparam HexLength
	//! @tparam WorldLength
	template <typename HexLength, typename WorldLength>
	struct hex_layout {
		using hex_length_t = HexLength;
		using world_length_t = WorldLength;
		using world_point_t = vecx::point<world_length_t, 2>;
		using world_vector_t = vecx::vector<world_length_t, 2>;

		hex_orientation orientation;
		world_vector_t size;
		world_point_t origin;

		//! Converts @p p to a world space point.
		constexpr auto to_world(hex_point<hex_length_t> p) const -> world_point_t {
			auto const x = (orientation.f0 * p.q + orientation.f1 * p.r) * size[0];
			auto const y = (orientation.f2 * p.q + orientation.f3 * p.r) * size[1];
			return origin + world_vector_t{world_length_t{x.value}, world_length_t{y.value}};
		}
		//! Converts @p v to a world space vector.
		constexpr auto to_world(hex_vector<hex_length_t> v) const -> world_vector_t {
			auto const x = (orientation.f0 * v.q + orientation.f1 * v.r) * size[0];
			auto const y = (orientation.f2 * v.q + orientation.f3 * v.r) * size[1];
			return {world_length_t{x.value}, world_length_t{y.value}};
		}

		//! Converts @p p to a hex space point.
		constexpr auto to_hex_space(world_point_t p) const -> hex_point<hex_length_t> {
			auto const r0 = orientation.b0 * (p[0] - origin[0]) / size[0];
			auto const r1 = orientation.b1 * (p[1] - origin[1]) / size[1];
			auto const r2 = orientation.b2 * (p[0] - origin[0]) / size[0];
			auto const r3 = orientation.b3 * (p[1] - origin[1]) / size[1];
			return {//
				hex_length_t{static_cast<typename hex_length_t::rep>((r0 + r1).value)},
				hex_length_t{static_cast<typename hex_length_t::rep>((r2 + r3).value)}};
		}
		//! Converts @p v to a hex space vector.
		constexpr auto to_hex_space(world_vector_t v) const -> hex_vector<hex_length_t> {
			auto const r0 = orientation.b0 * v[0] / size[0];
			auto const r1 = orientation.b1 * v[1] / size[1];
			auto const r2 = orientation.b2 * v[0] / size[0];
			auto const r3 = orientation.b3 * v[1] / size[1];
			return {hex_length_t{static_cast<int>((r0 + r1).value)}, hex_length_t{static_cast<int>((r2 + r3).value)}};
		}

		auto corner_offset(int corner) const -> world_vector_t {
			vecx::radians const angle = corner * vecx::circle_rad / 6.0 + orientation.start_angle;
			return {size[0] * static_cast<float>(cos(angle.value)), size[1] * static_cast<float>(sin(angle.value))};
		}

		//! A range containing the offsets from center of the six corners of hexes in this layout.
		auto corner_offsets() const& {
			return ranges::views::ints(0, 6) | ranges::views::transform([this](int i) { return corner_offset(i); });
		}
	};
}
