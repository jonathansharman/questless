//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "vector.hpp"

namespace units {
	template <typename... Quantities>
	struct point {
		constexpr int n = sizeof...(Quantities);

		std::tuple<Quantities...> components;

		//! Rotates the point about another point, overwriting the original value.
		//! @param origin The origin around which the point will be rotated.
		//! @param dphi The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle>
		void rotate(point const& origin, angle<UnitsPerCircle> const& dphi) & {
			static_assert(std::is_floating_point_v<scalar> && n == 2, "Requires two-dimensional floating-point space.");

			scalar const dphi_radians = angle_cast<radians>(dphi).count();
			auto const cos_dphi = static_cast<scalar>(cos(dphi_radians));
			auto const sin_dphi = static_cast<scalar>(sin(dphi_radians));
			auto offset = vector{origin[0], origin[1]};
			*this -= offset;
			*this = point
				{ static_cast<scalar>(this->_elements[0] * cos_dphi - this->_elements[1] * sin_dphi)
				, static_cast<scalar>(this->_elements[0] * sin_dphi + this->_elements[1] * cos_dphi)
				};
			*this += offset;
		}

		//! Creates a copy of the point, rotated about another point.
		//! @param origin The origin around which the point will be rotated.
		//! @param phi The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle>
		point rotated(point const& origin, angle<UnitsPerCircle> const& dphi) const {
			//! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
			static_assert(std::is_floating_point_v<scalar> && n == 2, "Requires two-dimensional floating-point space.");

			scalar const dphi_radians = angle_cast<radians>(dphi).count();
			auto const cos_dphi = static_cast<scalar>(cos(dphi_radians));
			auto const sin_dphi = static_cast<scalar>(sin(dphi_radians));
			auto const offset = vector{origin[0], origin[1]};
			point result = *this - offset;
			result = point
				{ static_cast<scalar>(result[0] * cos_dphi - result[1] * sin_dphi)
				, static_cast<scalar>(result[0] * sin_dphi + result[1] * cos_dphi)
				};
			result += offset;
			return result;
		}

		//! The vector's length.
		//! @note For better performance, use @p length_squared if possible, avoiding a sqrt.
		constexpr scalar length() const { return scalar{math::sqrt(length_squared().value)}; }

		//! The square of the vector's length.
		constexpr scalar length_squared() const {
			auto result = math::square(scalar(0));
			for (auto const& e : this->_elements) { result += e * e; }
			return result;
		}

		//! The counter-clockwise angle of the vector from the positive x-axis (in radians by default).
		template <typename UnitsPerCircle = radians::units_per_circle>
		typename space::angle<UnitsPerCircle> angle() const {
			//! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
			static_assert(n == 2, "Requires two-dimensional space.");
			return typename space::angle<UnitsPerCircle>{static_cast<scalar>(atan2(this->_elements[1], this->_elements[0]))};
		}
	};

	//! Template deduction guide for vector.
	template <typename... Quantities>
	point(Quantities...) -> point<Quantities...>;

	template <typename... Quantities>
	vector<Quantities...> operator +(vector<Quantities...> const& v1, vector<Quantities...> const& v2) {
		auto result = v1;
		result += v2;
		return result;
	}

	template <typename... Quantities>
	vector<Quantities...> operator -(vector<Quantities...> const& v1, vector<Quantities...> const& v2) {
		auto result = v1;
		result -= v2;
		return result;
	}

	template <typename... Quantities>
	vector<Quantities...> operator -(vector<Quantities...> const& v) {
		auto result = v;
		for (int i = 0; i < n; ++i) {
			result[i] = -result[i];
		}
		return result;
	}

	//! Performs @p BinaryOp on the elements of vectors @p v1 and @p v2 at index @p Index, returning the result.
	template <std::size_t Index, typename BinaryOp, typename... Quantities1, typename... Quantities2>
	constexpr auto component_bin_op(BinaryOp&& f, vector<Quantities1...> const& v1, std::tuple<Quantities2...> const& v2) {
		return std::forward<BinaryOp>(f)(v1.get<Index>(), v2.get<Index>());
	}

	//! Performs @p BinaryOp on the elements of vectors @p v1 and @p v2 at each of the indices @p Indices, returning the resulting vector.
	template <std::size_t... Indices, typename BinaryOp, typename... Quantities1, typename... Quantities2>
	constexpr auto component_wise_bin_op(std::index_sequence<Indices...>, BinaryOp&& f, vector<Quantities1...> const& v1, vector<Quantities2...> const& v2) {
		return vector{component_bin_op<Indices>(std::forward<BinaryOp>(f), v1, v2)...};
	}

	//! Performs component-wise @p BinaryOp on @p v1 and @p v2, returning the resulting vector.
	template <typename BinaryOp, typename... Quantities1, typename... Quantities2>
	constexpr auto component_wise_bin_op(BinaryOp&& f, vector<Quantities1...> const& v1, vector<Quantities2...> const& v2) {
		constexpr std::size_t size1 = sizeof...(Quantities1);
		constexpr std::size_t size2 = sizeof...(Quantities2);
		static_assert(size1 == size2, "Vectors must have the same size in order to perform a binary operation on their elements.");
		return component_wise_bin_op(std::make_index_sequence<size1>{}, std::forward<BinaryOp>(f), v1, v2);
	}

	//! Performs component-wise multiplication of @p v1 and @p v2, returning the resulting vector.
	template <typename... Quantities1, typename... Quantities2>
	constexpr auto component_wise_product(vector<Quantities1...> const& v1, vector<Quantities2...> const& v2) {
		return component_wise_binary_op(std::multiplies{}, v1, v2);
	}

	//! Performs component-wise division of @p v1 and @p v2, returning the resulting vector.
	template <typename... Quantities1, typename... Quantities2>
	constexpr auto component_wise_division(vector<Quantities1...> const& v1, vector<Quantities2...> const& v2) {
		return component_wise_binary_op(std::divides{}, v1, v2);
	}

	template <typename K, typename... Quantities>
	constexpr auto operator *(vector<Quantities...> const& v, K const& k) {
		return vector{}
		auto result = v;
		result *= k;
		return result;
	}
	template <typename K, typename... Quantities>
	vector<Quantities...> operator *(K const& k, vector<Quantities...> const& v) {
		auto result = v;
		for (int i = 0; i < n; ++i) {
			result[i] = k * result[i];
		}
		return result;
	}

	template <typename T, typename... Quantities>
	vector<Quantities...> operator /(vector<Quantities...> const& v, T const& k) {
		auto result = v;
		result /= k;
		return result;
	}




















	//! A point in this space.
	class point : public Buffer {
	public:
		constexpr point() = default;
		constexpr point(point const&) = default;
		constexpr point(point&&) = default;

		constexpr point(std::array<scalar, n> coordinates)
			: Buffer{std::move(coordinates)} {}

		template <typename... Args>
		constexpr point(Args... args) : Buffer{{std::forward<Args>(args)...}} {
			static_assert(sizeof...(args) == n, "All coordinates of a point must be initialized.");
		}

		point& operator =(point const&) & = default;
		point& operator =(point&&) & = default;

		constexpr scalar& operator [](std::size_t index) { return this->_elements[index]; }
		constexpr scalar operator [](std::size_t index) const { return this->_elements[index]; }

		template <typename Archive>
		void save(Archive& archive) const { archive(this->_elements); }

		template <typename Archive>
		void load(Archive& archive) { archive(this->_elements); }

		constexpr bool operator ==(point const& that) const {
			for (std::size_t i = 0; i < n; ++i) {
				if (this->_elements[i] != that._elements[i]) return false;
			}
			return true;
		}
		constexpr bool operator !=(point const& that) const {
			for (std::size_t i = 0; i < n; ++i) {
				if (this->_elements[i] != that._elements[i]) return true;
			}
			return false;
		}

		constexpr friend point operator +(point const& p, vector const& v) {
			point result;
			for (int i = 0; i < n; ++i) {
				result[i] = p[i] + v[i];
			}
			return result;
		}
		constexpr friend point operator +(vector const& v, point const& p) {
			point result;
			for (int i = 0; i < n; ++i) {
				result[i] = p[i] + v[i];
			}
			return result;
		}

		constexpr friend point operator -(point const& p, vector const& v) {
			point result;
			for (int i = 0; i < n; ++i) {
				result[i] = p[i] - v[i];
			}
			return result;
		}
		constexpr friend vector operator -(point const& p1, point const& p2) {
			vector result;
			for (int i = 0; i < n; ++i) {
				result[i] = p1[i] - p2[i];
			}
			return result;
		}

		friend point componentwise_product(point const& p, vector const& v) {
			auto result = p;
			result *= v;
			return result;
		}
		friend point componentwise_product(vector const& v, point const& p) {
			auto result = p;
			for (int i = 0; i < n; ++i) {
				result[i] = result[i] * v[i];
			}
			return result;
		}

		point& operator +=(vector const& v) & {
			for (int i = 0; i < n; ++i) {
				this->_elements[i] += v[i];
			}
			return *this;
		}
		point& operator -=(vector const& v) & {
			for (int i = 0; i < n; ++i) {
				this->_elements[i] -= v[i];
			}
			return *this;
		}

		//! Component-wise product.
		point& operator *=(vector const& v) {
			for (int i = 0; i < n; ++i) {
				this->_elements[i] *= v[i];
			}
			return *this;
		}

		//! Rotates the point about another point, overwriting the original value.
		//! @param origin The origin around which the point will be rotated.
		//! @param dphi The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle>
		void rotate(point const& origin, angle<UnitsPerCircle> const& dphi) & {
			static_assert(std::is_floating_point_v<scalar> && n == 2, "Requires two-dimensional floating-point space.");

			scalar const dphi_radians = angle_cast<radians>(dphi).count();
			auto const cos_dphi = static_cast<scalar>(cos(dphi_radians));
			auto const sin_dphi = static_cast<scalar>(sin(dphi_radians));
			auto offset = vector{origin[0], origin[1]};
			*this -= offset;
			*this = point
			{static_cast<scalar>(this->_elements[0] * cos_dphi - this->_elements[1] * sin_dphi)
			, static_cast<scalar>(this->_elements[0] * sin_dphi + this->_elements[1] * cos_dphi)
			};
			*this += offset;
		}

		//! Creates a copy of the point, rotated about another point.
		//! @param origin The origin around which the point will be rotated.
		//! @param phi The counter-clockwise rotation to apply, in radians.
		template <typename UnitsPerCircle>
		point rotated(point const& origin, angle<UnitsPerCircle> const& dphi) const {
			//! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
			static_assert(std::is_floating_point_v<scalar> && n == 2, "Requires two-dimensional floating-point space.");

			scalar const dphi_radians = angle_cast<radians>(dphi).count();
			auto const cos_dphi = static_cast<scalar>(cos(dphi_radians));
			auto const sin_dphi = static_cast<scalar>(sin(dphi_radians));
			auto const offset = vector{origin[0], origin[1]};
			point result = *this - offset;
			result = point
			{static_cast<scalar>(result[0] * cos_dphi - result[1] * sin_dphi)
			, static_cast<scalar>(result[0] * sin_dphi + result[1] * cos_dphi)
			};
			result += offset;
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

TEST_CASE("[point] operations") {
	using int_space = units::space<struct int_space_tag, int, 2>;
	using double_space = units::space<struct double_space_tag, double, 2>;

	typename int_space::point pi1{3, 4};

	SUBCASE("binary operations") {
		SUBCASE("addition") {
			constexpr int_space::vector vi{1, 6};
			constexpr int_space::point p_sum{4, 10};
			CHECK(pi1 + vi == p_sum);
			pi1 += vi;
			CHECK(pi1 == p_sum);
		}
		SUBCASE("subtraction") {
			constexpr int_space::point pi2{1, 6};
			constexpr int_space::vector v_diff{2, -2};
			CHECK(pi1 - pi2 == v_diff);

			constexpr int_space::vector vi{1, 6};
			constexpr int_space::point p_diff{2, -2};
			pi1 -= vi;
			CHECK(pi1 == p_diff);
		}
	}
	SUBCASE("rotations") {
		typename double_space::point pd{3.0, 4.0};
		constexpr typename double_space::degrees dphi{90.0};
		constexpr typename double_space::point pivot{1.0, 1.0};
		constexpr typename double_space::point pd_rot{-2.0, 3.0};
		auto pd_rot_actual = pd.rotated(pivot, dphi);

		CHECK(pd_rot_actual[0] == doctest::Approx(pd_rot[0]));
		CHECK(pd_rot_actual[1] == doctest::Approx(pd_rot[1]));

		pd.rotate(pivot, dphi);

		CHECK(pd[0] == doctest::Approx(pd_rot[0]));
		CHECK(pd[1] == doctest::Approx(pd_rot[1]));
	}
}
