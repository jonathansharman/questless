//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <array>
#include <initializer_list>
#include <ratio>

#include "constants.h"
#include "math.h"

namespace units
{
	template <typename ScalarType, int DimensionCount>
	class BaseBuffer
	{
	protected:
		constexpr BaseBuffer() = default;
		constexpr BaseBuffer(BaseBuffer const&) = default;
		constexpr BaseBuffer(BaseBuffer&&) = default;

		constexpr explicit BaseBuffer(std::array<ScalarType, DimensionCount> elements)
			: _elements{std::move(elements)}
		{}

		template <typename... Args>
		constexpr explicit BaseBuffer(Args... args)
			: _elements{{std::forward<Args>(args)...}}
		{}

		BaseBuffer& operator =(BaseBuffer const&) & = default;
		BaseBuffer& operator =(BaseBuffer&&) & = default;

		std::array<ScalarType, DimensionCount> _elements;
	};

	//! A generic type-safe space, parameterized over a number of dimensions and a scalar type.
	//! @tparam Tag A tag used to distinguish this space semantically from other structurally equivalent spaces.
	//! @tparam ScalarType The data type of scalar values in this space.
	//! @tparam DimensionCount The number of dimensions in this space. Must be non-negative.
	//! @tparam Buffer A type containing a @p std::array<ScalarType, DimensionCount>. May include space-spefific data accessors for use in points and vectors.
	template <typename Tag, typename ScalarType, int DimensionCount, typename Buffer = BaseBuffer<ScalarType, DimensionCount>>
	struct Space
	{
	private:
		//! @todo Is there a way to generate the type for the alignment tuple without using a dummy function?

		template <int... DimensionIndices>
		static decltype(auto) align_dummy(std::integer_sequence<int, DimensionIndices...>)
		{
			return std::tuple<typename Axis<DimensionIndices>::Align...>{};
		}

		// Private constructor to prevent instantiation.
		Space() = default;
	public:
		using scalar_t = ScalarType;
		static constexpr int dimension_count = DimensionCount;

		static_assert(dimension_count >= 0, "Number of dimensions must be non-negative.");

		//! Represents the axis along the dimension with index @p dimension.
		template <int dimension>
		struct Axis
		{
			static_assert(0 <= dimension && dimension < dimension_count, "Axis index out of bounds.");

			//! Alignment along this axis.
			enum class Align { near, mid, far };
		};

		//! The alignment along every axis in this space.
		using Alignment = decltype(align_dummy(std::make_integer_sequence<int, dimension_count>{}));

		//! An angle in this space, with @p UnitsPerCircle angle units per full circle, as a std::ratio.
		template <typename UnitsPerCircle>
		class Angle
		{
		public:
			using space_t = Space;
			using units_per_circle = UnitsPerCircle;

			//! The number of this angle unit in one full circle.
			static constexpr Angle circle() { return Angle{static_cast<scalar_t>(units_per_circle::num) / static_cast<scalar_t>(units_per_circle::den)}; }

			constexpr Angle() = default;
			constexpr Angle(Angle const&) = default;
			constexpr Angle(Angle&&) = default;
			constexpr explicit Angle(scalar_t count) : _count{std::move(count)} {}

			Angle& operator =(Angle const&) & = default;
			Angle& operator =(Angle&&) & = default;

			//! The zero angle for the specified space and unit size.
			static constexpr Angle zero() { return Angle{scalar_t(0)}; }

			constexpr scalar_t const& count() const& { return _count; }
			scalar_t&& count() && { return std::move(_count); }
			scalar_t& count() & { return _count; }

			friend std::ostream& operator <<(std::ostream& out, Angle const& theta)
			{
				out << theta.count();
				return out;
			}

			constexpr friend bool operator <(Angle const& theta1, Angle const& theta2) { return theta1._count < theta2._count; }
			constexpr friend bool operator <=(Angle const& theta1, Angle const& theta2) { return theta1._count <= theta2._count; }
			constexpr friend bool operator ==(Angle const& theta1, Angle const& theta2) { return theta1._count == theta2._count; }
			constexpr friend bool operator !=(Angle const& theta1, Angle const& theta2) { return theta1._count != theta2._count; }
			constexpr friend bool operator >=(Angle const& theta1, Angle const& theta2) { return theta1._count >= theta2._count; }
			constexpr friend bool operator >(Angle const& theta1, Angle const& theta2) { return theta1._count > theta2._count; }

			// Closed under addition.
			constexpr friend Angle operator +(Angle const& theta1, Angle const& theta2)
			{
				return Angle{theta1.count() + theta2.count()};
			}
			Angle& operator +=(Angle const& theta) &
			{
				_count += theta._count;
				return *this;
			}

			// Closed under substraction.
			constexpr friend Angle operator -(Angle const& theta1, Angle const& theta2)
			{
				return Angle{theta1.count() - theta2.count()};
			}
			Angle& operator -=(Angle const& theta) &
			{
				_count -= theta._count;
				return *this;
			}

			// Closed under negation.
			constexpr friend Angle operator -(Angle const& theta)
			{
				return Angle{-theta.count()};
			}

			// Closed under scalar multiplication.
			constexpr friend Angle operator *(scalar_t k, Angle const& theta)
			{
				return Angle{k * theta.count()};
			}
			constexpr friend Angle operator *(Angle const& theta, scalar_t k)
			{
				return Angle{k * theta.count()};
			}
			Angle& operator *=(scalar_t k) &
			{
				_count *= k;
				return *this;
			}

			// Closed under scalar division.
			constexpr friend Angle operator /(Angle const& theta, scalar_t k)
			{
				return Angle{theta.count() / k};
			}
			Angle& operator /=(scalar_t k) &
			{
				_count /= k;
				return *this;
			}
		private:
			scalar_t _count;
		};

		//! Casts @p theta from @p FromAngle to @p ToAngle.
		template<typename ToAngle, typename FromAngle>
		static constexpr ToAngle angle_cast(FromAngle const& theta)
		{
			using ratio = std::ratio_divide<ToAngle::units_per_circle, FromAngle::units_per_circle>;
			using from_scalar_t = typename FromAngle::space_t::scalar_t;
			using to_scalar_t = typename ToAngle::space_t::scalar_t;
			return ToAngle{static_cast<to_scalar_t>(theta.count() * static_cast<from_scalar_t>(ratio::num) / static_cast<from_scalar_t>(ratio::den))};
		}

		using Radians = Angle<std::ratio<static_cast<int>(constants::tau * 1'000'000), 1'000'000>>;

		using Degrees = Angle<std::ratio<360>>;

		using Gradians = Angle<std::ratio<400>>;

		//! A vector in this space.
		class Vector : public Buffer
		{
		public:
			constexpr Vector() = default;
			constexpr Vector(Vector const&) = default;
			constexpr Vector(Vector&&) = default;

			constexpr explicit Vector(std::array<scalar_t, dimension_count> components)
				: Buffer{std::move(components)}
			{}

			template <typename... Args>
			constexpr explicit Vector(Args... args)
				: Buffer{{std::forward<Args>(args)...}}
			{
				static_assert(sizeof...(args) == dimension_count, "All components of a Vector must be initialized.");
			}

			template <typename UnitsPerCircle>
			explicit Vector(Angle<UnitsPerCircle> theta, scalar_t r) //! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
				: Buffer
					{ static_cast<scalar_t>(r * cos(theta.count()))
					, static_cast<scalar_t>(r * sin(theta.count()))
					}
			{
				static_assert(dimension_count == 2, "Requires two-dimensional space.");
			}

			Vector& operator =(Vector const&) & = default;
			Vector& operator =(Vector&&) & = default;

			//! The zero vector for this space.
			static constexpr Vector zero()
			{
				return Vector{std::array<scalar_t, dimension_count>()};
			}

			scalar_t& operator [](std::size_t index) & { return _elements[index]; }
			constexpr scalar_t const& operator [](std::size_t index) const& { return _elements[index]; }

			friend std::ostream& operator <<(std::ostream& out, Vector const& v)
			{
				out << '(';
				for (std::size_t i = 0; i < v._elements.size() - 1; ++i) {
					out << v[i] << ", ";
				}
				out << v._elements.back() << ')';
				return out;
			}

			bool operator ==(Vector const& that) const { return _elements == that._elements; }
			bool operator !=(Vector const& that) const { return _elements != that._elements; }

			friend Vector operator +(Vector const& v1, Vector const& v2)
			{
				auto result = v1;
				result += v2;
				return result;
			}

			friend Vector operator -(Vector const& v1, Vector const& v2)
			{
				auto result = v1;
				result -= v2;
				return result;
			}
			friend Vector operator -(Vector const& v)
			{
				auto result = v;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = -result[i];
				}
				return result;
			}

			//! Component-wise product.
			friend Vector operator *(Vector const& v1, Vector const& v2)
			{
				auto result = v1;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] *= v2[i];
				}
				return result;
			}
			friend Vector operator *(Vector const& v, scalar_t k)
			{
				auto result = v;
				result *= k;
				return result;
			}
			friend Vector operator *(scalar_t k, Vector const& v)
			{
				auto result = v;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = k * result[i];
				}
				return result;
			}

			friend Vector operator /(Vector const& v, scalar_t k)
			{
				auto result = v;
				result /= k;
				return result;
			}

			Vector& operator +=(Vector const& that) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] += that[i];
				}
				return *this;
			}
			Vector& operator -=(Vector const& that) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] -= that[i];
				}
				return *this;
			}
			Vector& operator *=(scalar_t k) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] *= k;
				}
				return *this;
			}
			Vector& operator /=(scalar_t k) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] /= k;
				}
				return *this;
			}

			//! Rotates the vector, overwriting the original value.
			//! @param dtheta The counter-clockwise rotation to apply, in radians.
			template <typename UnitsPerCircle>
			void rotate(Angle<UnitsPerCircle> const& dtheta) &
			{
				static_assert(std::is_floating_point_v<scalar_t> && dimension_count == 2, "Requires two-dimensional floating-point space.");

				scalar_t const dtheta_radians = angle_cast<Radians>(dtheta).count();
				auto const cos_dtheta = static_cast<scalar_t>(cos(dtheta_radians));
				auto const sin_dtheta = static_cast<scalar_t>(sin(dtheta_radians));
				*this = Vector
					{ static_cast<scalar_t>(_elements[0] * cos_dtheta - _elements[1] * sin_dtheta)
					, static_cast<scalar_t>(_elements[0] * sin_dtheta + _elements[1] * cos_dtheta)
					};
			}

			//! Creates a rotated copy of the vector.
			//! @param dtheta The counter-clockwise rotation to apply, in radians.
			template <typename UnitsPerCircle>
			Vector rotated(Angle<UnitsPerCircle> const& dtheta) const //! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
			{
				static_assert(std::is_floating_point_v<scalar_t> && dimension_count == 2, "Requires two-dimensional floating-point space.");

				scalar_t const dtheta_radians = angle_cast<Radians>(dtheta).count();
				auto const cos_dtheta = static_cast<scalar_t>(cos(dtheta_radians));
				auto const sin_dtheta = static_cast<scalar_t>(sin(dtheta_radians));
				return Vector
					{ static_cast<scalar_t>(_elements[0] * cos_dtheta - _elements[1] * sin_dtheta)
					, static_cast<scalar_t>(_elements[0] * sin_dtheta + _elements[1] * cos_dtheta)
					};
			}

			//! The vector's length.
			constexpr scalar_t length() const
			{
				scalar_t sum_of_squares(0);
				for (int i = 0; i < dimension_count; ++i) {
					sum_of_squares += _elements[i] * _elements[i];
				}
				return static_cast<scalar_t>(math::sqrt(sum_of_squares));
			}

			//! The square of the vector's length.
			constexpr scalar_t length_squared() const
			{
				scalar_t result(0);
				for (int i = 0; i < dimension_count; ++i) {
					result += _elements[i] * _elements[i];
				}
				return result;
			}

			//! The counter-clockwise angle of the vector from the positive x-axis (in radians by default).
			template <typename UnitsPerCircle = Radians::units_per_circle>
			Angle<UnitsPerCircle> angle() const  //! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
			{
				static_assert(dimension_count == 2, "Requires two-dimensional space.");
				return Angle<UnitsPerCircle>{static_cast<scalar_t>(atan2(_elements[1], _elements[0]))};
			}
		};

		//! A point in this space.
		class Point : public Buffer
		{
		public:
			constexpr Point() = default;
			constexpr Point(Point const&) = default;
			constexpr Point(Point&&) = default;

			constexpr explicit Point(std::array<scalar_t, dimension_count> coordinates)
				: Buffer{std::move(coordinates)}
			{}

			template <typename... Args>
			constexpr explicit Point(Args... args)
				: Buffer{{std::forward<Args>(args)...}}
			{
				static_assert(sizeof...(args) == dimension_count, "All coordinates of a Point must be initialized.");
			}

			Point& operator =(Point const&) & = default;
			Point& operator =(Point&&) & = default;

			scalar_t& operator [](std::size_t index) & { return _elements[index]; }
			constexpr scalar_t const& operator [](std::size_t index) const& { return _elements[index]; }

			friend std::ostream& operator <<(std::ostream& out, Point const& p)
			{
				out << '(';
				for (std::size_t i = 0; i < dimension_count - 1; ++i) {
					out << p[i] << ", ";
				}
				out << p[dimension_count - 1] << ')';
				return out;
			}

			constexpr bool operator ==(Point const& that) const
			{
				for (int i = 0; i < dimension_count; ++i) {
					if (_elements[i] != that[i]) {
						return false;
					}
				}
				return true;
			}
			constexpr bool operator !=(Point const& that) const
			{
				for (int i = 0; i < dimension_count; ++i) {
					if (_elements[i] != that[i]) {
						return true;
					}
				}
				return false;
			}

			constexpr friend Point operator +(Point const& p, Vector const& v)
			{
				Point result;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = p[i] + v[i];
				}
				return result;
			}
			constexpr friend Point operator +(Vector const& v, Point const& p)
			{
				Point result;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = p[i] + v[i];
				}
				return result;
			}

			constexpr friend Point operator -(Point const& p, Vector const& v)
			{
				Point result;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = p[i] - v[i];
				}
				return result;
			}
			constexpr friend Vector operator -(Point const& p1, Point const& p2)
			{
				Vector result;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = p1[i] - p2[i];
				}
				return result;
			}

			//! Component-wise product.
			friend Point operator *(Point const& p, Vector const& v)
			{
				auto result = p;
				result *= v;
				return result;
			}
			//! Component-wise product.
			friend Point operator *(Vector const& v, Point const& p)
			{
				auto result = p;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = result[i] * v[i];
				}
				return result;
			}

			Point& operator +=(Vector const& v) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] += v[i];
				}
				return *this;
			}
			Point& operator -=(Vector const& v) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] -= v[i];
				}
				return *this;
			}

			//! Component-wise product.
			Point& operator *=(Vector const& v)
			{
				for (int i = 0; i < dimension_count; ++i) {
					coordinates[i] = coordinates[i] * v[i];
				}
				return *this;
			}

			//! Rotates the point about another point, overwriting the original value.
			//! @param origin The origin around which the point will be rotated.
			//! @param dtheta The counter-clockwise rotation to apply, in radians.
			template <typename UnitsPerCircle>
			void rotate(Point const& origin, Angle<UnitsPerCircle> const& dtheta) &
			{
				static_assert(std::is_floating_point_v<scalar_t> && dimension_count == 2, "Requires two-dimensional floating-point space.");

				scalar_t const dtheta_radians = angle_cast<Radians>(dtheta).count();
				auto const cos_dtheta = static_cast<scalar_t>(cos(dtheta_radians));
				auto const sin_dtheta = static_cast<scalar_t>(sin(dtheta_radians));
				auto offset = Vector{origin[0], origin[1]};
				*this -= offset;
				*this = Point
					{ static_cast<scalar_t>(_elements[0] * cos_dtheta - _elements[1] * sin_dtheta)
					, static_cast<scalar_t>(_elements[0] * sin_dtheta + _elements[1] * cos_dtheta)
					};
				*this += offset;
			}

			//! Creates a copy of the point, rotated about another point.
			//! @param origin The origin around which the point will be rotated.
			//! @param theta The counter-clockwise rotation to apply, in radians.
			template <typename UnitsPerCircle>
			Point rotated(Point const& origin, Angle<UnitsPerCircle> const& dtheta) const //! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
			{
				static_assert(std::is_floating_point_v<scalar_t> && dimension_count == 2, "Requires two-dimensional floating-point space.");

				scalar_t const dtheta_radians = angle_cast<Radians>(dtheta).count();
				auto const cos_dtheta = static_cast<scalar_t>(cos(dtheta_radians));
				auto const sin_dtheta = static_cast<scalar_t>(sin(dtheta_radians));
				auto const offset = Vector{origin[0], origin[1]};
				Point result = *this - offset;
				result = Point
					{ static_cast<scalar_t>(result[0] * cos_dtheta - result[1] * sin_dtheta)
					, static_cast<scalar_t>(result[0] * sin_dtheta + result[1] * cos_dtheta)
					};
				result += offset;
				return result;
			}
		};

		//! A rectangular, orthogonal volume in this space.
		class Box
		{
		public:
			//! The position of the minimal corner of this box.
			Point position;

			//! The size or extent of this box.
			Vector size;

			constexpr Box() = default;
			constexpr Box(Box const&) = default;
			constexpr Box(Box&&) = default;

			//! A box of size @p size with minimal corner at @p position.
			explicit constexpr Box(Point position, Vector size)
				: position{std::move(position)}
				, size{std::move(size)}
			{}

			//! A box of size @p size at @p position with alignment @p alignment.
			//! @param position The position of this box relative to its origin.
			//! @param size The size of this box.
			//! @param alignment The alignment of each of this box's axes, which determines where the origin is.
			explicit constexpr Box(Point position, Vector size, Alignment const& alignment)
				: position{aligned_position(position, size, alignment)}
				, size{std::move(size)}
			{}

			Box& operator =(Box const&) & = default;
			Box& operator =(Box&&) & = default;

			friend std::ostream& operator <<(std::ostream& out, Box const& box)
			{
				out << '(' << box.origin << ", " << box.size << ')';
				return out;
			}

			constexpr friend bool operator ==(Box const& left, Box const& right)
			{
				return left.origin == right.origin && left.size == right.size;
			}
			constexpr friend bool operator !=(Box const& left, Box const& right)
			{
				return left.origin != right.origin || left.size != right.size;
			}

			constexpr bool contains(Point const& point) const
			{
				//! @todo Rigorously define what containment means for generic spaces (inclusive vs. exclusive bounds).
				for (int i = 0; i < dimension_count; ++i) {
					if (point[i] < position[i] || point[i] > position[i] + size[i]) {
						return false;
					}
				}
				return true;
			}

			//! Translates the rectangle by the given offset.
			void translate(Vector const& offset) &
			{
				position += offset;
			}

			//! A copy of this box translated by @p offset.
			constexpr Box translated(Vector const& offset) const
			{
				return Box{position + offset, size};
			}

			//! Extends this box, if necessary, to include the given point.
			void extend(Point const& point)
			{
				for (int i = 0; i < dimension_count; ++i) {
					if (point[i] < position[i]) {
						size[i] += position[i] - point[i];
						position[i] = point[i];
					} else if (point[i] > position[i] + size[i]) {
						size[i] = point[i] - position[i];
					}
				}
			}
		private:
			static constexpr Point aligned_position(Point position, Vector size, Alignment const& alignment)
			{
				return apply_alignment<0>(position, size, alignment);
			}

			template <int index>
			static constexpr Point apply_alignment(Point position, Vector size, Alignment const& alignment)
			{
				//! @todo Can use constexpr-if for base case here, when supported.

				//! @todo Specialize these adjustments for integers and floats?
				switch (std::get<index>(alignment)) {
					case Axis<index>::Align::near:
						break;
					case Axis<index>::Align::mid:
						position[index] -= size[index] / 2 - 1;
						break;
					case Axis<index>::Align::far:
						position[index] -= size[index] - 1;
						break;
				}
				return apply_alignment<index + 1>(position, size, alignment);
			}

			template <>
			static constexpr Point apply_alignment<dimension_count>(Point position, Vector, Alignment const&)
			{
				return position;
			}
		};
	};
}

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest.h>

TEST_CASE("[Angle] operations")
{
	using DoubleSpace = units::Space<struct DoubleSpaceTag, double, 2>;

	SUBCASE("conversions")
	{
		constexpr auto circle_degrees = DoubleSpace::angle_cast<DoubleSpace::Degrees>(DoubleSpace::Radians::circle());
		CHECK(circle_degrees.count() == doctest::Approx(360.0));
	}
}

TEST_CASE("[Vector] operations")
{
	using IntSpace = units::Space<struct IntSpaceTag, int, 2>;
	using DoubleSpace = units::Space<struct DoubleSpaceTag, double, 2>;

	IntSpace::Vector vi1{3, 4};

	SUBCASE("length")
	{
		CHECK(vi1.length() == 5);
		CHECK(vi1.length_squared() == 25);
	}
	SUBCASE("angle")
	{
		constexpr DoubleSpace::Vector vd{3.0, 4.0};
		CHECK(vd.angle<DoubleSpace::Radians>().count() == doctest::Approx(0.927295218));
	}
	SUBCASE("binary operations")
	{
		constexpr IntSpace::Vector vi2{1, 6};
		SUBCASE("addition")
		{
			constexpr IntSpace::Vector v_sum{4, 10};
			CHECK(vi1 + vi2 == v_sum);
			vi1 += vi2;
			CHECK(vi1 == v_sum);
		}
		SUBCASE("subtraction")
		{
			constexpr IntSpace::Vector v_diff{2, -2};
			CHECK(vi1 - vi2 == v_diff);
			vi1 -= vi2;
			CHECK(vi1 == v_diff);
		}
	}
	SUBCASE("scalar operations")
	{
		constexpr int k = 2;
		SUBCASE("scalar multiplication")
		{
			constexpr IntSpace::Vector v_prod{6, 8};
			CHECK(k * vi1 == v_prod);
			CHECK(vi1 * k == v_prod);
			vi1 *= k;
			CHECK(vi1 == v_prod);
		}
		SUBCASE("scalar division")
		{
			constexpr IntSpace::Vector v_quot{1, 2};
			CHECK(vi1 / k == v_quot);
			vi1 /= k;
			CHECK(vi1 == v_quot);
		}
	}
	SUBCASE("rotations")
	{
		DoubleSpace::Vector vd{3.0, 4.0};
		constexpr DoubleSpace::Degrees dtheta{90.0};
		constexpr DoubleSpace::Vector vd_rot{-4.0, 3.0};
		auto vd_rot_actual = vd.rotated(dtheta);

		CHECK(vd_rot_actual[0] == doctest::Approx(vd_rot[0]));
		CHECK(vd_rot_actual[1] == doctest::Approx(vd_rot[1]));

		vd.rotate(dtheta);

		CHECK(vd[0] == doctest::Approx(vd_rot[0]));
		CHECK(vd[1] == doctest::Approx(vd_rot[1]));
	}
}

TEST_CASE("[Point] operations")
{
	using IntSpace = units::Space<struct IntSpaceTag, int, 2>;
	using DoubleSpace = units::Space<struct DoubleSpaceTag, double, 2>;

	IntSpace::Point pi1{3, 4};

	SUBCASE("binary operations")
	{
		SUBCASE("addition")
		{
			constexpr IntSpace::Vector vi{1, 6};
			constexpr IntSpace::Point p_sum{4, 10};
			CHECK(pi1 + vi == p_sum);
			pi1 += vi;
			CHECK(pi1 == p_sum);
		}
		SUBCASE("subtraction")
		{
			constexpr IntSpace::Point pi2{1, 6};
			constexpr IntSpace::Vector v_diff{2, -2};
			CHECK(pi1 - pi2 == v_diff);

			constexpr IntSpace::Vector vi{1, 6};
			constexpr IntSpace::Point p_diff{2, -2};
			pi1 -= vi;
			CHECK(pi1 == p_diff);
		}
	}
	SUBCASE("rotations")
	{
		DoubleSpace::Point pd{3.0, 4.0};
		constexpr DoubleSpace::Degrees dtheta{90.0};
		constexpr DoubleSpace::Point pivot{1.0, 1.0};
		constexpr DoubleSpace::Point pd_rot{-2.0, 3.0};
		auto pd_rot_actual = pd.rotated(pivot, dtheta);

		CHECK(pd_rot_actual[0] == doctest::Approx(pd_rot[0]));
		CHECK(pd_rot_actual[1] == doctest::Approx(pd_rot[1]));

		pd.rotate(pivot, dtheta);

		CHECK(pd[0] == doctest::Approx(pd_rot[0]));
		CHECK(pd[1] == doctest::Approx(pd_rot[1]));
	}
}

//! @todo Box tests
