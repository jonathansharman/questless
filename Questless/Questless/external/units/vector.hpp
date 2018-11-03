//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "angle.hpp"
#include "constants.hpp"

#include "meta/quantity.hpp"

#include <gcem.hpp>

#include <algorithm>
#include <tuple>

namespace units {
	template <typename... Quantities>
	struct vector {
		//! The number of dimensions of this vector type.
		static constexpr std::size_t n = sizeof...(Quantities);

		std::tuple<Quantities...> components;

		template <typename... Args>
		constexpr vector(Args&&... args) : components{std::forward<Args>(args)...} {}

		explicit constexpr vector(std::tuple<Quantities...> components) : components{std::move(components)} {}

		template <typename Archive>
		void save(Archive& archive) const { archive(components); }

		template <typename Archive>
		void load(Archive& archive) { archive(components); }

		//! Gets the component at index @p Index.
		template <std::size_t Index>
		constexpr auto& get() { return std::get<Index>(components); }

		//! Gets the component at index @p Index.
		template <std::size_t Index>
		constexpr auto get() const { return std::get<Index>(components); }

		//! Gets the component with type @p Quantity. @p Quantity must be unique in this vector.
		template <typename Quantity>
		constexpr auto& get() { return std::get<Quantity>(components); }

		//! Gets the component with type @p Quantity. @p Quantity must be unique in this vector.
		template <typename Quantity>
		constexpr auto get() const { return std::get<Quantity>(components); }

		//! Calls @p f on each component of this vector, in order.
		template <typename Consumer>
		constexpr void for_each(Consumer const& f) const {
			std::apply([this, f](auto const&... components) { (f(components), ...); }, components);
		}
		//! Calls @p f on each component of this vector, in order.
		template <typename Consumer>
		constexpr void for_each(Consumer const& f) {
			std::apply([this, f](auto&... components) { (f(components), ...); }, components);
		}

		//! Creates a new vector by applying @p f to each component of this vector, in order.
		template <typename MapOp>
		constexpr auto map(MapOp const& f) const {
			return std::apply([this, f](auto const&... components) {
				return vector{f(components)...};
			}, components);
		}

		//! Creates a new vector from the application of @p f to the corresponding components of this vector and @p that, for each index in @p Indices.
		template <std::size_t... Indices, typename ZipOp, typename... ThatQuantities>
		constexpr auto zip(ZipOp const& f, vector<ThatQuantities...> const& that, std::index_sequence<Indices...>) const {
			static_assert(n == sizeof...(ThatQuantities), "Cannot zip vectors with different numbers of dimensions.");
			return vector{f(get<Indices>(), that.get<Indices>())...};
		}

		//! Creates a new vector from the application of @p f to the corresponding components of this vector and @p that.
		template <typename ZipOp, typename... ThatQuantities>
		constexpr auto zip(ZipOp const& f, vector<ThatQuantities...> const& that) const {
			return zip(f, that, std::make_index_sequence<n>{});
		}

		//! Calls @p f on each corresponding component of this vector and @p that, for each index in @p Indices.
		template <std::size_t... Indices, typename MergeOp, typename... ThatQuantities>
		constexpr void for_each_pairwise(MergeOp const& f, vector<ThatQuantities...> const& that, std::index_sequence<Indices...>) {
			static_assert(n == sizeof...(ThatQuantities), "Cannot perform pair-wise iteration on vectors with different numbers of dimensions.");
			(f(get<Indices>(), that.get<Indices>()), ...);
		}

		//! Calls @p f on each corresponding component of this vector and @p that.
		template <typename MergeOp, typename... ThatQuantities>
		constexpr void for_each_pairwise(MergeOp const& f, vector<ThatQuantities...> const& that) {
			for_each_pairwise(f, that, std::make_index_sequence<n>{});
		}

		//! Produces a left reduction/fold over this vector.
		//! @param f The operation to apply to the accumulator and the next vector component, to produce the next accumulator value.
		//! @param accumulator The starting value of the reduction.
		template <std::size_t Index = 0, typename T, typename ReduceOp>
		constexpr auto reduce(ReduceOp const& f, T const& accumulator) const {
			return Index == n - 1
				? f(accumulator, get<Index>()) // base case
				: reduce<Index + 1>(f, f(accumulator, get<Index>())) // recursive case
				;
		}

		template <typename... ThatQuantities>
		constexpr vector<Quantities...>& operator +=(vector<ThatQuantities...> const& that) {
			for_each_pairwise([](auto& component1, auto const& component2) {
				component1 += component2;
			}, that);
			return *this;
		}

		template <typename... ThatQuantities>
		constexpr vector<Quantities...>& operator -=(vector<ThatQuantities...> const& that) {
			for_each_pairwise([](auto& component1, auto const& component2) {
				component1 -= component2;
			}, that);
			return *this;
		}

		template <typename K>
		constexpr auto& operator *=(K const& k) {
			for_each([&k](auto& component) { component *= k; });
			return *this;
		}

		template <typename K>
		constexpr auto& operator /=(K const& k) {
			for_each([&k](auto& component) { component /= k; });
			return *this;
		}

		//! Rotates this vector by @p angle in the @p Axis1 to @p Axis2 plane.
		template <std::size_t Axis1 = 0, std::size_t Axis2 = Axis1 + 1>
		constexpr void rotate(radians angle) {
			auto const cos_angle = gcem::cos(angle.value);
			auto const sin_angle = gcem::sin(angle.value);
			auto const old_axis1 = get<Axis1>();
			get<Axis1>() = static_cast<decltype(get<Axis1>())>(old_axis1 * cos_angle - get<Axis2>() * sin_angle);
			get<Axis2>() = static_cast<decltype(get<Axis2>())>(old_axis1 * sin_angle + get<Axis2>() * cos_angle);
		}

		template <std::size_t... Indices>
		constexpr void rotate(std::array<radians, sizeof...(Indices)> angles, std::index_sequence<Indices...>) {
			(rotate<Indices>(angles[Indices]), ...);
		}

		//! Rotates this vector by each of @p angles, in successive planes.
		template <typename... Radians>
		constexpr void rotate(Radians... angles) {
			rotate(std::array{angles...}, std::make_index_sequence<sizeof...(Radians)>{});
		}

		//! Creates a copy of the vector rotated by @p angle.
		constexpr vector<Quantities...> rotated(radians angle) const {
			auto result = *this;
			result.rotate(angle);
			return result;
		}

		//! The square of the vector's length.
		constexpr auto length_squared() const {
			using acc_t = std::tuple_element_t<0, std::tuple<Quantities...>>;
			return reduce([](auto const& acc, auto const& component) {
				return acc + component * component;
			}, acc_t(0) * acc_t(0));
		}

		//! The vector's length. Only defined if all components have the same units.
		//! @note For better performance, use @p length_squared if possible, avoiding a sqrt.
		constexpr auto length() const { return sqrt(length_squared().value); }

		//! The angle of this vector in the @p Axis1 to @p Axis2 plane, in radians.
		template <std::size_t Axis1 = 0, std::size_t Axis2 = 1>
		constexpr radians angle() const {
			static_assert
				( std::is_convertible_v<std::tuple_element_t<Axis1, std::tuple<Quantities...>>::rep, double>
					&& std::is_convertible_v<std::tuple_element_t<Axis2, std::tuple<Quantities...>>::rep, double>
				, "Both axes' quantity representations must be convertible to double to find the angle between them."
				);
			auto atan2 = [](double x, double y) {
				return
					// Positive x half plane: just use atan().
					x > 0.0 ? gcem::atan(y / x) :
					// Negative x half plane: need to adjust atan() result.
					x < 0.0 ? gcem::atan(y / x) + (y >= 0.0 ? constants::pi : -constants::pi) :
					// On the y-axis: answer is pi or -pi.
					y > 0.0 ? constants::pi / 2 : -constants::pi / 2;
			};
			return radians{atan2(static_cast<double>(get<Axis2>().value), static_cast<double>(get<Axis1>().value))};
		}
	};

	//! Template deduction guide for variadic vector construction.
	template <typename... Quantities>
	vector(Quantities...) -> vector<Quantities...>;

	//! Template deduction guide for vector construction from tuple.
	template <typename... Quantities>
	vector(std::tuple<Quantities...>) -> vector<Quantities...>;

	//! Concatenates two vectors.
	template <typename... Quantities1, typename... ThatQuantities>
	auto vector_cat(vector<Quantities1...> const& v1, vector<ThatQuantities...> const& v2) {
		return vector{std::tuple_cat(v1.components, v2.components)};
	}

	namespace detail {
		// Used to allow expansion of variadic index template parameter with another type.
		template <typename T, std::size_t>
		using ignore_size_t = T;

		//! Constructs a zero vector with as many axes as the size of @p Indices, with each axis of type @p Quantity.
		template <typename Quantity, std::size_t... Indices>
		constexpr auto make_zero_vector(std::make_index_sequence<Indices...>) {
			return vector{detail::ignore_size_t<Quantity, Indices>(0)...};
		}
	}

	//! Constructs a zero vector of dimension @p N, with each axis of type @p Quantity.
	template <typename Quantity, std::size_t N>
	constexpr auto make_zero_vector() {
		return detail::make_zero_vector(std::make_index_sequence<N>{});
	}

	//! Constructs a vector from a magnitude and direction.
	//! @param magnitude The magnitude or length of the vector. Must be a quantity.
	//! @param angles The counterclockwise rotations to apply to the vector, in radians, following the order of the axes.
	template <typename Quantity, typename... Angles>
	constexpr auto make_polar_vector(Quantity magnitude, Angles... angles) {
		auto result = make_zero_vector<Quantity, sizeof...(Angles)>();
		result.get<0>() = magnitude;
		result.rotate(angles...);
		return result;
	}

	//! The vector sum of @p v1 and @p v2.
	template <typename... Quantities>
	constexpr vector<Quantities...> operator +(vector<Quantities...> const& v1, vector<Quantities...> const& v2) {
		return v1.zip([](auto const& component1, auto const& component2) {
			return component1 + component2;
		}, v2);
	}

	//! The vector difference of @p v1 and @p v2.
	template <typename... Quantities>
	constexpr vector<Quantities...> operator -(vector<Quantities...> const& v1, vector<Quantities...> const& v2) {
		return v1.zip([](auto const& component1, auto const& component2) {
			return component1 - component2;
		}, v2);
	}

	//! The negation of @p v.
	template <typename... Quantities>
	constexpr vector<Quantities...> operator -(vector<Quantities...> const& v) {
		return v.map([](auto const& component) { return -component; });
	}

	//! Performs component-wise multiplication of @p v1 and @p v2, returning the resulting vector.
	template <typename... Quantities1, typename... Quantities2>
	constexpr auto component_wise_product(vector<Quantities1...> const& v1, vector<Quantities2...> const& v2) {
		return v1.zip(std::multiplies{}, v2);
	}

	//! Performs component-wise division of @p v1 and @p v2, returning the resulting vector.
	template <typename... Quantities1, typename... Quantities2>
	constexpr auto component_wise_division(vector<Quantities1...> const& v1, vector<Quantities2...> const& v2) {
		return v1.zip(std::divides{}, v2);
	}

	//! Multiplies a vector @p v by a scalar value @p k.
	template <typename K, typename... Quantities>
	constexpr auto operator *(vector<Quantities...> const& v, K const& k) {
		return v.map([&k](auto const& component) { return component * k; });
	}
	//! Multiplies a scalar value @p k by a vector @p v.
	template <typename K, typename... Quantities>
	constexpr vector<Quantities...> operator *(K const& k, vector<Quantities...> const& v) {
		return v.map([&k](auto const& component) { return k * component; });
	}

	//! Divides a vector @p v by a scalar value @p k.
	template <typename T, typename... Quantities>
	constexpr vector<Quantities...> operator /(vector<Quantities...> const& v, T const& k) {
		return v.map([&k](auto const& component) { return component / k; });
	}
}

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest.h>
#undef near // Defined in minwindef.h (!)
#undef far // Defined in minwindef.h (!)

TEST_CASE("[vector] operations") {
	using namespace units;
	using namespace meta;

	using u_t = unit_t<struct u_t_tag>;
	using q_t = quantity<int, u_t>;
	using q2_t = product_t<q_t, q_t>;
	using v_t = vector<q_t, q_t>;

	v_t v1{q_t{3}, q_t{4}};

	SUBCASE("length") {
		CHECK(v1.length() == q_t{5});
		CHECK(v1.length_squared() == q2_t{25});
	}
	SUBCASE("angle") {
		CHECK(v1.angle().value == doctest::Approx(0.927295218));
	}
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
			v_t expected{q2_t{6}, q2_t{8}};
			CHECK(k * v1 == expected);
			CHECK(v1 * k == expected);
			v1 *= k;
			CHECK(v1 == expected);
		}
		SUBCASE("scalar division") {
			constexpr auto expected = vector{unitless<int>{1}, unitless<int>{2}};
			CHECK(v1 / k == expected);
			v1 /= k;
			CHECK(v1 == expected);
		}
	}
	SUBCASE("rotations") {
		v_t v{q_t{3}, q_t{4}};
		auto theta = 90.0_deg;
		v_t expected{q_t{-4}, q_t{3}};

		auto v_rotated = v.rotated(theta * rad_per_deg);
		CHECK(v_rotated == expected);

		v.rotate(theta);
		CHECK(v == expected);
	}
}
