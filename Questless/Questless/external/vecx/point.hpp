//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "vector.hpp"

namespace vecx {
	//! Represents an n-dimensional point of quantities.
	//! @tparam Quantity The quantity type of scalars in this point type.
	//! @tparam N The dimension of this point type.
	template <typename Quantity, std::size_t N>
	struct point {
		//! The type of scalars in this point type.
		using scalar_t = Quantity;

		//! The number of dimensions of this point type.
		static constexpr std::size_t n = N;

		std::array<scalar_t, n> components{};

		constexpr point() = default;
		constexpr point(point const&) = default;
		constexpr point(point&&) = default;

		template <typename... Args, typename = std::enable_if_t<(std::is_convertible_v<Args, scalar_t> && ...)>>
		constexpr point(Args && ... args) : components{std::forward<Args>(args)...} {}

		constexpr point& operator =(point const&) = default;
		constexpr point& operator =(point&&) = default;

		//! The canonical origin point for this point type, viz. all zeroes.
		static constexpr auto origin() { return point<scalar_t, n>{}; }

		template <typename ThatQuantity>
		constexpr bool operator ==(point<ThatQuantity, n> const& that) const {
			for (std::size_t i = 0; i < n; ++i) {
				if (components[i] != that[i]) return false;
			}
			return true;
		}

		//! Gets the component at index @p index.
		constexpr auto& operator [](std::size_t index) { return components[index]; }

		//! Gets the component at index @p index.
		constexpr auto operator [](std::size_t index) const { return components[index]; }

		//! Creates a new point by applying @p f to each component of this point, in order.
		template <typename MapOp>
		constexpr auto map(MapOp const& f) const {
			return std::apply([this, f](auto const& ... components) {
				return vector<std::invoke_result_t<MapOp, scalar_t>, n>{f(components)...};
				}, components);
		}

		//! Creates a new point from the application of @p f to the corresponding components of this point and @p that.
		template <typename ThatQuantity, typename ZipOp>
		constexpr auto zip(vector<ThatQuantity, n> const& that, ZipOp const& f) const {
			auto result = point<scalar_t::rep, scalar_t::unit, n>::zero();
			for (std::size_t i = 0; i < n; ++i) {
				result[i] = f(components[i], that.components[i]);
			}
			return result;
		}

		//! Produces a left reduction/fold over this point.
		//! @param f The operation to apply to the accumulator and the next point component, to produce the next accumulator value.
		//! @param accumulator The starting value of the reduction.
		template <typename T, typename ReduceOp>
		constexpr auto reduce(T accumulator, ReduceOp const& f) const {
			//! @todo Replace this function with std::reduce/std::accumulate once those are made constexpr.
			for (auto const& component : components) {
				accumulator = f(std::move(accumulator), component);
			}
			return accumulator;
		}

		template <typename Quantity>
		constexpr auto& operator +=(vector<Quantity, n> const& v) {
			for (std::size_t i = 0; i < n; ++i) {
				components[i] += v[i];
			}
			return *this;
		}

		template <typename Quantity>
		constexpr auto& operator -=(vector<Quantity, n> const& v) {
			for (std::size_t i = 0; i < n; ++i) {
				components[i] -= v[i];
			}
			return *this;
		}

		//! Creates a copy of this point, rotated around another point.
		//! @param origin The origin around which to rotate this point.
		//! @param angle The angle of the rotation, from @p axis1 to @p axis2.
		constexpr auto rotated(point<scalar_t, n> const& origin, radians angle, std::size_t axis1 = 0, std::size_t axis2 = 1) const {
			return origin + (*this - origin).rotated(angle, axis1, axis2);
		}

		//! Creates a copy of this point, rotated around @p origin by each of @p angles, in successive planes.
		constexpr auto rotated(point<scalar_t, n> const& origin, std::array<radians, n - 1> const& angles) const {
			return origin + (*this - origin).rotated(angles);
		}

		//! Rotates this point around another point.
		//! @param origin The origin around which to rotate this point.
		//! @param angle The angle of the rotation, from @p axis1 to @p axis2.
		constexpr void rotate(point<scalar_t, n> const& origin, radians angle, std::size_t axis1 = 0, std::size_t axis2 = 1) {
			*this = rotated(origin, angle, axis1, axis2);
		}

		//! Rotates this point around @p origin by each of @p angles, in successive planes.
		constexpr void rotate(point<scalar_t, n> const& origin, std::array<radians, n - 1> const& angles) {
			*this = rotated(origin, angles);
		}
	};

	template <typename T, typename... U>
	point(T, U...)->point<std::remove_cv_t<std::remove_reference_t<T>>, 1 + sizeof...(U)>;

	//! The sum of point @p p and vector @p v.
	template <typename QuantityPoint, typename QuantityVector, std::size_t N>
	constexpr auto operator +(point<QuantityPoint, N> const& p, vector<QuantityVector, N> const& v) {
		auto result = p;
		result += v;
		return result;
	}

	//! The sum of vector @p v and point @p p.
	template <typename QuantityVector, typename QuantityPoint, std::size_t N>
	constexpr auto operator +(vector<QuantityVector, N> const& v, point<QuantityPoint, N> const& p) {
		auto result = p;
		for (std::size_t i = 0; i < N; ++i) {
			result[i] = v[i] + p[i];
		}
		return result;
	}

	//! The vector difference of @p p1 and @p p2.
	template <typename Quantity1, typename Quantity2, std::size_t N>
	constexpr auto operator -(point<Quantity1, N> const& p1, point<Quantity2, N> const& p2) {
		auto result = vector<Quantity1, N>::zero();
		for (std::size_t i = 0; i < N; ++i) {
			result[i] = p1[i] - p2[i];
		}
		return result;
	}
}

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest/doctest.h>
#undef near // Defined in minwindef.h (!)
#undef far // Defined in minwindef.h (!)

TEST_CASE("[point] operations") {
	using namespace cancel;
	using namespace vecx;
	using namespace vecx::literals;

	using u_t = unit_t<struct u_t_tag>;
	using q_t = quantity<int, u_t>;
	using q2_t = product_t<q_t, q_t>;
	using v_t = vector<q_t, 2>;
	using p_t = vector<q_t, 2>;

	v_t v1{q_t{3}, q_t{4}};
	p_t p1{q_t{3}, q_t{4}};

	SUBCASE("binary operations") {
		constexpr v_t v2{q_t{1}, q_t{6}};
		SUBCASE("addition") {
			constexpr v_t expected{q_t{4}, q_t{10}};
			CHECK(v1 + v2 == expected);
			v1 += v2;
			CHECK(v1 == expected);
		}
		SUBCASE("subtraction") {
			constexpr v_t expected{q_t{2}, q_t{-2}};
			CHECK(v1 - v2 == expected);
			v1 -= v2;
			CHECK(v1 == expected);
		}
	}
	SUBCASE("scalar operations") {
		q_t k = q_t{2};
		SUBCASE("scalar multiplication") {
			constexpr auto expected = vector{q2_t{6}, q2_t{8}};
			CHECK(k * v1 == expected);
			CHECK(v1 * k == expected);
		}
		SUBCASE("scalar division") {
			constexpr auto expected = vector{unitless<int>{1}, unitless<int>{2}};
			CHECK(v1 / k == expected);
		}
	}
	SUBCASE("rotations") {
		v_t v{q_t{3}, q_t{4}};
		auto theta = 90.0_deg;
		v_t expected{q_t{-4}, q_t{3}};

		auto v_rotated = v.rotated(theta * rad_per_deg);
		CHECK(v_rotated == expected);

		v.rotate(theta * rad_per_deg);
		CHECK(v == expected);
	}
}
