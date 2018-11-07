//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "angle.hpp"
#include "constants.hpp"

#include "meta/quantity.hpp"

#include <gcem.hpp>

#include <algorithm>
#include <array>

namespace units {
	//! Represents an n-dimensional vector of quantities.
	//! @tparam Quantity The quantity type of scalars in this vector type.
	//! @tparam N The dimension of this vector type.
	template <typename Quantity, std::size_t N>
	struct vector {
		//! The type of scalars in this vector type.
		using scalar_t = Quantity;

		//! The number of dimensions of this vector type.
		static constexpr std::size_t n = N;

		std::array<scalar_t, n> components{};

		template <typename... Args>
		constexpr vector(Args&&... args) : components{{std::forward<Args>(args)...}} {}

		explicit constexpr vector(std::array<scalar_t, n> components) : components{std::move(components)} {}

		//! The zero vector for this vector type.
		static constexpr auto zero() { return vector<scalar_t, n>{}; }

		template <typename ThatQuantity>
		constexpr bool operator ==(vector<ThatQuantity, n> const& that) {
			for (std::size_t i = 0; i < n; ++i) {
				if (components[i] != that[i]) return false;
			}
			return true;
		}

		template <typename ThatQuantity>
		constexpr bool operator !=(vector<ThatQuantity, n> const& that) {
			for (std::size_t i = 0; i < n; ++i) {
				if (components[i] != that[i]) return true;
			}
			return false;
		}

		template <typename Archive>
		void save(Archive& archive) const { archive(components); }

		template <typename Archive>
		void load(Archive& archive) { archive(components); }

		//! Gets the component at index @p index.
		constexpr auto& operator [](std::size_t index) { return components[index]; }

		//! Gets the component at index @p index.
		constexpr auto operator [](std::size_t index) const { return components[index]; }

		auto begin() { return components.begin(); }
		auto end() { return components.end(); }

		auto begin() const { return components.begin(); }
		auto end() const { return components.end(); }

		//! Creates a new vector by applying @p f to each component of this vector, in order.
		template <typename MapOp>
		constexpr auto map(MapOp const& f) const {
			return std::apply([this, f](auto const&... components) {
				return vector{f(components)...};
			}, components);
		}

		//! Creates a new vector from the application of @p f to the corresponding components of this vector and @p that.
		template <typename ThatQuantity, typename ZipOp>
		constexpr auto zip(vector<ThatQuantity, n> const& that, ZipOp const& f) const {
			auto result = vector<scalar_t, n>::zero();
			for (std::size_t i = 0; i < n; ++i) {
				result[i] = f(components[i], that.components[i]);
			}
			return result;
		}

		//! Produces a left reduction/fold over this vector.
		//! @param f The operation to apply to the accumulator and the next vector component, to produce the next accumulator value.
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
		constexpr auto& operator +=(vector<Quantity, n> const& that) {
			for (std::size_t i = 0; i < n; ++i) {
				components[i] += that[i];
			}
			return *this;
		}

		template <typename Quantity>
		constexpr auto& operator -=(vector<Quantity, n> const& that) {
			for (std::size_t i = 0; i < n; ++i) {
				components[i] -= that[i];
			}
			return *this;
		}

		template <typename K>
		constexpr auto& operator *=(K const& k) {
			for (auto& component : components) {
				component *= k;
			}
			return *this;
		}

		template <typename K>
		constexpr auto& operator /=(K const& k) {
			for (auto& component : components) {
				component /= k;
			}
			return *this;
		}

		//! Rotates this vector by @p angle in the @p axis1 to @p axis2 plane.
		constexpr void rotate(radians angle, std::size_t axis1 = 0, std::size_t axis2 = 1) {
			auto const cos_angle = gcem::cos(angle.value);
			auto const sin_angle = gcem::sin(angle.value);
			auto const old_axis1 = components[axis1];
			components[axis1] = static_cast<scalar_t>(old_axis1 * cos_angle - components[axis2] * sin_angle);
			components[axis2] = static_cast<scalar_t>(old_axis1 * sin_angle + components[axis2] * cos_angle);
		}

		//! Rotates this vector by each of @p angles, in successive planes.
		constexpr void rotate(std::array<radians, n - 1> const& angles) {
			for (std::size_t i = 0; i < n; ++i) {
				rotate(angles[i], i, i + 1);
			}
		}

		//! Creates a copy of this vector, rotated by @p angle.
		constexpr auto rotated(radians angle) const {
			auto result = *this;
			result.rotate(angle);
			return result;
		}

		//! Creates a copy of this vector, rotated by each of @p angles, in successive planes.
		constexpr auto rotated(std::array<radians, n - 1> const& angles) const {
			auto result = *this;
			result.rotate(angles);
			return result;
		}

		//! The square of the vector's length.
		constexpr auto length_squared() const {
			return reduce(scalar_t(0) * scalar_t(0), [](auto const& acc, auto const& component) {
				return acc + component * component;
			});
		}

		//! The vector's length. Only defined if all components have the same units.
		//! @note For better performance, use @p length_squared if possible, avoiding a sqrt.
		constexpr auto length() const { return sqrt(length_squared()); }

		//! The angle of this vector in the @p axis1 to @p axis2 plane, in radians.
		constexpr radians angle(std::size_t axis1 = 0, std::size_t axis2 = 1) const {
			static_assert
				( std::is_convertible_v<scalar_t::rep, double>
				, "Scalar representation must be convertible to double to find the vector angle."
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
			return radians{atan2(static_cast<double>(components[axis2].value), static_cast<double>(components[axis1].value))};
		}
	};

	template <typename T, typename... U>
	vector(T, U...) -> vector<T, 1 + sizeof...(U)>;

	template <typename Quantity, std::size_t N>
	vector(std::array<Quantity, N>) -> vector<Quantity, N>;

	//! Constructs a vector from a magnitude and direction.
	//! @param magnitude The magnitude or length of the vector. Must be a quantity.
	//! @param angles The counterclockwise rotations to apply to the vector, in radians, following the order of the axes.
	template <typename Quantity, std::size_t N, typename... Angles>
	constexpr auto make_polar_vector(Quantity const& magnitude, std::array<radians, N> const& angles) {
		return vector<Quantity, N>{magnitude}.rotated(angles);
	}

	//! The vector sum of @p v1 and @p v2.
	template <typename Quantity1, typename Quantity2, std::size_t N>
	constexpr auto operator +(vector<Quantity1, N> const& v1, vector<Quantity2, N> const& v2) {
		return v1.zip(v2, std::plus{});
	}

	//! The vector difference of @p v1 and @p v2.
	template <typename Quantity1, typename Quantity2, std::size_t N>
	constexpr auto operator -(vector<Quantity1, N> const& v1, vector<Quantity2, N> const& v2) {
		return v1.zip(v2, std::minus{});
	}

	//! The negation of @p v.
	template <typename Quantity, std::size_t N>
	constexpr auto operator -(vector<Quantity, N> const& v) {
		return v.map(std::negate{});
	}

	//! Performs component-wise multiplication of @p v1 and @p v2, returning the resulting vector.
	template <typename Quantity1, typename Quantity2, std::size_t N>
	constexpr auto component_wise_product(vector<Quantity1, N> const& v1, vector<Quantity2, N> const& v2) {
		return v1.zip(v2, std::multiplies{});
	}

	//! Performs component-wise division of @p v1 and @p v2, returning the resulting vector.
	template <typename Quantity1, typename Quantity2, std::size_t N>
	constexpr auto component_wise_quotient(vector<Quantity1, N> const& v1, vector<Quantity2, N> const& v2) {
		return v1.zip(std::divides{}, v2);
	}

	//! Multiplies a vector @p v by a scalar value @p k.
	template <typename Quantity, std::size_t N, typename K>
	constexpr auto operator *(vector<Quantity, N> const& v, K const& k) {
		return v.map([&k](auto const& component) { return component * k; });
	}
	//! Multiplies a scalar value @p k by a vector @p v.
	template <typename K, typename Quantity, std::size_t N>
	constexpr auto operator *(K const& k, vector<Quantity, N> const& v) {
		return v.map([&k](auto const& component) { return k * component; });
	}

	//! Divides a vector @p v by a scalar value @p k.
	template <typename Quantity, std::size_t N, typename K>
	constexpr auto operator /(vector<Quantity, N> const& v, K const& k) {
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
	using namespace meta;
	using namespace units;
	using namespace units::literals;

	using u_t = unit_t<struct u_t_tag>;
	using q_t = quantity<int, u_t>;
	using q2_t = product_t<q_t, q_t>;
	using v_t = vector<q_t, 2>;

	v_t v1{q_t{3}, q_t{4}};

	SUBCASE("length") {
		CHECK(v1.length().value == 5);
		CHECK(v1.length_squared().value == 25);
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

		v.rotate(theta);
		CHECK(v == expected);
	}
}
