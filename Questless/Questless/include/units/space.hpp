//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <array>
#include <initializer_list>
#include <ratio>

#include "constants.hpp"
#include "math.hpp"

namespace units
{
	template <typename Scalar, int DimensionCount>
	class buffer_base
	{
	protected:
		constexpr buffer_base() = default;
		constexpr buffer_base(buffer_base const&) = default;
		constexpr buffer_base(buffer_base&&) = default;

		constexpr explicit buffer_base(std::array<Scalar, DimensionCount> elements)
			: _elements{std::move(elements)}
		{}

		template <typename... Args>
		constexpr explicit buffer_base(Args... args)
			: _elements{{std::forward<Args>(args)...}}
		{}

		buffer_base& operator =(buffer_base const&) & = default;
		buffer_base& operator =(buffer_base&&) & = default;

		std::array<Scalar, DimensionCount> _elements;
	};

	//! A generic type-safe space, parameterized over a number of dimensions and a scalar type.
	//! @tparam Tag A tag used to distinguish this space semantically from other structurally equivalent spaces.
	//! @tparam Scalar The data type of scalar values in this space.
	//! @tparam DimensionCount The number of dimensions in this space. Must be non-negative.
	//! @tparam Buffer A type containing a @p std::array<Scalar, DimensionCount>. May include space-spefific data accessors for use in points and vectors.
	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer = buffer_base<Scalar, DimensionCount>>
	struct space
	{
	private:
		//! @todo Is there a way to generate the type for the alignment tuple without using a dummy function?

		template <int... DimensionIndices>
		static decltype(auto) align_dummy(std::integer_sequence<int, DimensionIndices...>)
		{
			return std::tuple<typename axis<DimensionIndices>::align...>{};
		}

		// Private constructor to prevent instantiation.
		space() = default;
	public:
		using scalar = Scalar;
		static constexpr int dimension_count = DimensionCount;

		static_assert(dimension_count >= 0, "Number of dimensions must be non-negative.");

		//! Represents the axis along the dimension with index @p dimension.
		template <int dimension>
		struct axis
		{
			//static_assert(0 <= dimension && dimension < dimension_count, "Axis index out of bounds.");

			//! Alignment along this axis.
			enum class align { near, mid, far };
		};

		//! The alignment along every axis in this space.
		using alignment = decltype(align_dummy(std::make_integer_sequence<int, dimension_count>{}));

		//! An angle in this space, with @p UnitsPerCircle angle units per full circle, as a std::ratio.
		template <typename UnitsPerCircle>
		class angle
		{
		public:
			using scalar = scalar;
			using units_per_circle = UnitsPerCircle;

			//! The number of this angle unit in one full circle.
			static constexpr angle circle() { return angle{static_cast<scalar>(units_per_circle::num) / static_cast<scalar>(units_per_circle::den)}; }

			constexpr angle() = default;
			constexpr angle(angle const&) = default;
			constexpr angle(angle&&) = default;
			constexpr explicit angle(scalar count) : _count{std::move(count)} {}

			angle& operator =(angle const&) & = default;
			angle& operator =(angle&&) & = default;

			//! The zero angle for the specified space and unit size.
			static constexpr angle zero() { return angle{scalar(0)}; }

			constexpr scalar const& count() const& { return _count; }
			scalar&& count() && { return std::move(_count); }
			scalar& count() & { return _count; }

			friend std::ostream& operator <<(std::ostream& out, angle const& theta)
			{
				out << theta.count();
				return out;
			}

			constexpr friend bool operator <(angle const& theta1, angle const& theta2) { return theta1._count < theta2._count; }
			constexpr friend bool operator <=(angle const& theta1, angle const& theta2) { return theta1._count <= theta2._count; }
			constexpr friend bool operator ==(angle const& theta1, angle const& theta2) { return theta1._count == theta2._count; }
			constexpr friend bool operator !=(angle const& theta1, angle const& theta2) { return theta1._count != theta2._count; }
			constexpr friend bool operator >=(angle const& theta1, angle const& theta2) { return theta1._count >= theta2._count; }
			constexpr friend bool operator >(angle const& theta1, angle const& theta2) { return theta1._count > theta2._count; }

			// Closed under addition.
			constexpr friend angle operator +(angle const& theta1, angle const& theta2)
			{
				return angle{theta1.count() + theta2.count()};
			}
			angle& operator +=(angle const& theta) &
			{
				_count += theta._count;
				return *this;
			}

			// Closed under substraction.
			constexpr friend angle operator -(angle const& theta1, angle const& theta2)
			{
				return angle{theta1.count() - theta2.count()};
			}
			angle& operator -=(angle const& theta) &
			{
				_count -= theta._count;
				return *this;
			}

			// Closed under negation.
			constexpr friend angle operator -(angle const& theta)
			{
				return angle{-theta.count()};
			}

			// Closed under scalar multiplication.
			constexpr friend angle operator *(scalar k, angle const& theta)
			{
				return angle{k * theta.count()};
			}
			constexpr friend angle operator *(angle const& theta, scalar k)
			{
				return angle{k * theta.count()};
			}
			angle& operator *=(scalar k) &
			{
				_count *= k;
				return *this;
			}

			// Closed under scalar division.
			constexpr friend angle operator /(angle const& theta, scalar k)
			{
				return angle{theta.count() / k};
			}
			angle& operator /=(scalar k) &
			{
				_count /= k;
				return *this;
			}
		private:
			scalar _count;
		};

		//! Casts @p theta from @p FromAngle to @p ToAngle.
		template<typename ToAngle, typename FromAngle>
		static constexpr ToAngle angle_cast(FromAngle const& theta)
		{
			using ratio = std::ratio_divide<ToAngle::units_per_circle, FromAngle::units_per_circle>;
			using from_scalar = typename FromAngle::scalar;
			using to_scalar = typename ToAngle::scalar;
			return ToAngle{static_cast<to_scalar>(theta.count() * static_cast<from_scalar>(ratio::num) / static_cast<from_scalar>(ratio::den))};
		}

		using radians = angle<std::ratio<static_cast<int>(constants::tau * 1'000'000), 1'000'000>>;

		using degrees = angle<std::ratio<360>>;

		using gradians = angle<std::ratio<400>>;

		//! A vector in this space.
		class vector : public Buffer
		{
		public:
			constexpr vector() = default;
			constexpr vector(vector const&) = default;
			constexpr vector(vector&&) = default;

			constexpr explicit vector(std::array<scalar, dimension_count> components)
				: Buffer{std::move(components)}
			{}

			template <typename... Args>
			constexpr explicit vector(Args... args)
				: Buffer{{std::forward<Args>(args)...}}
			{
				static_assert(sizeof...(args) == dimension_count, "All components of a vector must be initialized.");
			}

			//! Constructs a vector with angle @p theta and radius @p r.
			template <typename UnitsPerCircle>
			explicit vector(typename space::angle<UnitsPerCircle> theta, scalar r) //! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
				: Buffer
					{ static_cast<scalar>(r * cos(theta.count()))
					, static_cast<scalar>(r * sin(theta.count()))
					}
			{
				static_assert(dimension_count == 2, "Requires two-dimensional space.");
			}

			vector& operator =(vector const&) & = default;
			vector& operator =(vector&&) & = default;

			//! The zero vector for this space.
			static constexpr vector zero()
			{
				return vector{std::array<scalar, dimension_count>()};
			}

			scalar& operator [](std::size_t index) & { return _elements[index]; }
			constexpr scalar const& operator [](std::size_t index) const& { return _elements[index]; }

			friend std::ostream& operator <<(std::ostream& out, vector const& v)
			{
				out << '(';
				for (std::size_t i = 0; i < v._elements.size() - 1; ++i) {
					out << v[i] << ", ";
				}
				out << v._elements.back() << ')';
				return out;
			}

			bool operator ==(vector const& that) const { return _elements == that._elements; }
			bool operator !=(vector const& that) const { return _elements != that._elements; }

			friend vector operator +(vector const& v1, vector const& v2)
			{
				auto result = v1;
				result += v2;
				return result;
			}

			friend vector operator -(vector const& v1, vector const& v2)
			{
				auto result = v1;
				result -= v2;
				return result;
			}
			friend vector operator -(vector const& v)
			{
				auto result = v;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = -result[i];
				}
				return result;
			}

			//! Component-wise product.
			friend vector operator *(vector const& v1, vector const& v2)
			{
				auto result = v1;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] *= v2[i];
				}
				return result;
			}
			friend vector operator *(vector const& v, scalar k)
			{
				auto result = v;
				result *= k;
				return result;
			}
			friend vector operator *(scalar k, vector const& v)
			{
				auto result = v;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = k * result[i];
				}
				return result;
			}

			friend vector operator /(vector const& v, scalar k)
			{
				auto result = v;
				result /= k;
				return result;
			}

			vector& operator +=(vector const& that) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] += that[i];
				}
				return *this;
			}
			vector& operator -=(vector const& that) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] -= that[i];
				}
				return *this;
			}
			vector& operator *=(scalar k) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] *= k;
				}
				return *this;
			}
			vector& operator /=(scalar k) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] /= k;
				}
				return *this;
			}

			//! Rotates the vector, overwriting the original value.
			//! @param dtheta The counter-clockwise rotation to apply, in radians.
			template <typename UnitsPerCircle>
			void rotate(typename space::angle<UnitsPerCircle> const& dtheta) &
			{
				static_assert(std::is_floating_point_v<scalar> && dimension_count == 2, "Requires two-dimensional floating-point space.");

				scalar const dtheta_radians = angle_cast<radians>(dtheta).count();
				auto const cos_dtheta = static_cast<scalar>(cos(dtheta_radians));
				auto const sin_dtheta = static_cast<scalar>(sin(dtheta_radians));
				*this = vector
					{ static_cast<scalar>(_elements[0] * cos_dtheta - _elements[1] * sin_dtheta)
					, static_cast<scalar>(_elements[0] * sin_dtheta + _elements[1] * cos_dtheta)
					};
			}

			//! Creates a rotated copy of the vector.
			//! @param dtheta The counter-clockwise rotation to apply, in radians.
			template <typename UnitsPerCircle>
			vector rotated(typename space::angle<UnitsPerCircle> const& dtheta) const //! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
			{
				static_assert(std::is_floating_point_v<scalar> && dimension_count == 2, "Requires two-dimensional floating-point space.");

				scalar const dtheta_radians = angle_cast<radians>(dtheta).count();
				auto const cos_dtheta = static_cast<scalar>(cos(dtheta_radians));
				auto const sin_dtheta = static_cast<scalar>(sin(dtheta_radians));
				return vector
					{ static_cast<scalar>(_elements[0] * cos_dtheta - _elements[1] * sin_dtheta)
					, static_cast<scalar>(_elements[0] * sin_dtheta + _elements[1] * cos_dtheta)
					};
			}

			//! The vector's length.
			constexpr scalar length() const
			{
				scalar sum_of_squares(0);
				for (int i = 0; i < dimension_count; ++i) {
					sum_of_squares += _elements[i] * _elements[i];
				}
				return static_cast<scalar>(math::sqrt(sum_of_squares));
			}

			//! The square of the vector's length.
			constexpr scalar length_squared() const
			{
				scalar result(0);
				for (int i = 0; i < dimension_count; ++i) {
					result += _elements[i] * _elements[i];
				}
				return result;
			}

			//! The counter-clockwise angle of the vector from the positive x-axis (in radians by default).
			template <typename UnitsPerCircle = radians::units_per_circle>
			typename space::angle<UnitsPerCircle> angle() const  //! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
			{
				static_assert(dimension_count == 2, "Requires two-dimensional space.");
				return typename space::angle<UnitsPerCircle>{static_cast<scalar>(atan2(_elements[1], _elements[0]))};
			}
		};

		//! A point in this space.
		class point : public Buffer
		{
		public:
			constexpr point() = default;
			constexpr point(point const&) = default;
			constexpr point(point&&) = default;

			constexpr explicit point(std::array<scalar, dimension_count> coordinates)
				: Buffer{std::move(coordinates)}
			{}

			template <typename... Args>
			constexpr explicit point(Args... args)
				: Buffer{{std::forward<Args>(args)...}}
			{
				static_assert(sizeof...(args) == dimension_count, "All coordinates of a point must be initialized.");
			}

			point& operator =(point const&) & = default;
			point& operator =(point&&) & = default;

			scalar& operator [](std::size_t index) & { return _elements[index]; }
			constexpr scalar const& operator [](std::size_t index) const& { return _elements[index]; }

			friend std::ostream& operator <<(std::ostream& out, point const& p)
			{
				out << '(';
				for (std::size_t i = 0; i < dimension_count - 1; ++i) {
					out << p[i] << ", ";
				}
				out << p[dimension_count - 1] << ')';
				return out;
			}

			constexpr bool operator ==(point const& that) const
			{
				for (int i = 0; i < dimension_count; ++i) {
					if (_elements[i] != that[i]) {
						return false;
					}
				}
				return true;
			}
			constexpr bool operator !=(point const& that) const
			{
				for (int i = 0; i < dimension_count; ++i) {
					if (_elements[i] != that[i]) {
						return true;
					}
				}
				return false;
			}

			constexpr friend point operator +(point const& p, vector const& v)
			{
				point result;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = p[i] + v[i];
				}
				return result;
			}
			constexpr friend point operator +(vector const& v, point const& p)
			{
				point result;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = p[i] + v[i];
				}
				return result;
			}

			constexpr friend point operator -(point const& p, vector const& v)
			{
				point result;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = p[i] - v[i];
				}
				return result;
			}
			constexpr friend vector operator -(point const& p1, point const& p2)
			{
				vector result;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = p1[i] - p2[i];
				}
				return result;
			}

			//! Component-wise product.
			friend point operator *(point const& p, vector const& v)
			{
				auto result = p;
				result *= v;
				return result;
			}
			//! Component-wise product.
			friend point operator *(vector const& v, point const& p)
			{
				auto result = p;
				for (int i = 0; i < dimension_count; ++i) {
					result[i] = result[i] * v[i];
				}
				return result;
			}

			point& operator +=(vector const& v) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] += v[i];
				}
				return *this;
			}
			point& operator -=(vector const& v) &
			{
				for (int i = 0; i < dimension_count; ++i) {
					_elements[i] -= v[i];
				}
				return *this;
			}

			//! Component-wise product.
			point& operator *=(vector const& v)
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
			void rotate(point const& origin, angle<UnitsPerCircle> const& dtheta) &
			{
				static_assert(std::is_floating_point_v<scalar> && dimension_count == 2, "Requires two-dimensional floating-point space.");

				scalar const dtheta_radians = angle_cast<radians>(dtheta).count();
				auto const cos_dtheta = static_cast<scalar>(cos(dtheta_radians));
				auto const sin_dtheta = static_cast<scalar>(sin(dtheta_radians));
				auto offset = vector{origin[0], origin[1]};
				*this -= offset;
				*this = point
					{ static_cast<scalar>(_elements[0] * cos_dtheta - _elements[1] * sin_dtheta)
					, static_cast<scalar>(_elements[0] * sin_dtheta + _elements[1] * cos_dtheta)
					};
				*this += offset;
			}

			//! Creates a copy of the point, rotated about another point.
			//! @param origin The origin around which the point will be rotated.
			//! @param theta The counter-clockwise rotation to apply, in radians.
			template <typename UnitsPerCircle>
			point rotated(point const& origin, angle<UnitsPerCircle> const& dtheta) const //! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
			{
				static_assert(std::is_floating_point_v<scalar> && dimension_count == 2, "Requires two-dimensional floating-point space.");

				scalar const dtheta_radians = angle_cast<radians>(dtheta).count();
				auto const cos_dtheta = static_cast<scalar>(cos(dtheta_radians));
				auto const sin_dtheta = static_cast<scalar>(sin(dtheta_radians));
				auto const offset = vector{origin[0], origin[1]};
				point result = *this - offset;
				result = point
					{ static_cast<scalar>(result[0] * cos_dtheta - result[1] * sin_dtheta)
					, static_cast<scalar>(result[0] * sin_dtheta + result[1] * cos_dtheta)
					};
				result += offset;
				return result;
			}
		};

		//! A rectangular, orthogonal volume in this space.
		class box
		{
		public:
			//! The position of the minimal corner of this box.
			point position;

			//! The size or extent of this box.
			vector size;

			constexpr box() = default;
			constexpr box(box const&) = default;
			constexpr box(box&&) = default;

			//! A box of size @p size with minimal corner at @p position.
			explicit constexpr box(point position, vector size)
				: position{std::move(position)}
				, size{std::move(size)}
			{}

			//! A box of size @p size at @p position with alignment @p alignment.
			//! @param position The position of this box relative to its origin.
			//! @param size The size of this box.
			//! @param alignment The alignment of each of this box's axes, which determines where the origin is.
			explicit constexpr box(point position, vector size, alignment const& alignment)
				: position{aligned_position(position, size, alignment)}
				, size{std::move(size)}
			{}

			box& operator =(box const&) & = default;
			box& operator =(box&&) & = default;

			friend std::ostream& operator <<(std::ostream& out, box const& box)
			{
				out << '(' << box.origin << ", " << box.size << ')';
				return out;
			}

			constexpr friend bool operator ==(box const& left, box const& right)
			{
				return left.origin == right.origin && left.size == right.size;
			}
			constexpr friend bool operator !=(box const& left, box const& right)
			{
				return left.origin != right.origin || left.size != right.size;
			}

			constexpr bool contains(point const& point) const
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
			void translate(vector const& offset) &
			{
				position += offset;
			}

			//! A copy of this box translated by @p offset.
			constexpr box translated(vector const& offset) const
			{
				return box{position + offset, size};
			}

			//! Extends this box, if necessary, to include the given point.
			void extend(point const& point)
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
			static constexpr point aligned_position(point position, vector size, alignment const& alignment)
			{
				return apply_alignment<0>(position, size, alignment);
			}

			template <int index>
			static constexpr point apply_alignment(point position, vector size, alignment const& alignment)
			{
				//! @todo Can use constexpr-if for base case here, when supported.

				//! @todo Specialize these adjustments for integers and floats?
				switch (std::get<index>(alignment)) {
					case axis<index>::align::near:
						break;
					case axis<index>::align::mid:
						position[index] -= size[index] / 2 - 1;
						break;
					case axis<index>::align::far:
						position[index] -= size[index] - 1;
						break;
				}
				return apply_alignment<index + 1>(position, size, alignment);
			}

			template <>
			static constexpr point apply_alignment<dimension_count>(point position, vector, alignment const&)
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

TEST_CASE("[angle] operations")
{
	using double_space = units::space<struct double_space_tag, double, 2>;

	SUBCASE("conversions")
	{
		constexpr auto circle_degrees = double_space::angle_cast<double_space::degrees>(double_space::radians::circle());
		CHECK(circle_degrees.count() == doctest::Approx(360.0));
	}
}

TEST_CASE("[vector] operations")
{
	using int_space = units::space<struct int_space_tag, int, 2>;
	using double_space = units::space<struct double_space_tag, double, 2>;

	int_space::vector vi1{3, 4};

	SUBCASE("length")
	{
		CHECK(vi1.length() == 5);
		CHECK(vi1.length_squared() == 25);
	}
	SUBCASE("angle")
	{
		constexpr double_space::vector vd{3.0, 4.0};
		CHECK(vd.angle<double_space::radians>().count() == doctest::Approx(0.927295218));
	}
	SUBCASE("binary operations")
	{
		constexpr int_space::vector vi2{1, 6};
		SUBCASE("addition")
		{
			constexpr int_space::vector v_sum{4, 10};
			CHECK(vi1 + vi2 == v_sum);
			vi1 += vi2;
			CHECK(vi1 == v_sum);
		}
		SUBCASE("subtraction")
		{
			constexpr int_space::vector v_diff{2, -2};
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
			constexpr int_space::vector v_prod{6, 8};
			CHECK(k * vi1 == v_prod);
			CHECK(vi1 * k == v_prod);
			vi1 *= k;
			CHECK(vi1 == v_prod);
		}
		SUBCASE("scalar division")
		{
			constexpr int_space::vector v_quot{1, 2};
			CHECK(vi1 / k == v_quot);
			vi1 /= k;
			CHECK(vi1 == v_quot);
		}
	}
	SUBCASE("rotations")
	{
		double_space::vector vd{3.0, 4.0};
		constexpr double_space::degrees dtheta{90.0};
		constexpr double_space::vector vd_rot{-4.0, 3.0};
		auto vd_rot_actual = vd.rotated(dtheta);

		CHECK(vd_rot_actual[0] == doctest::Approx(vd_rot[0]));
		CHECK(vd_rot_actual[1] == doctest::Approx(vd_rot[1]));

		vd.rotate(dtheta);

		CHECK(vd[0] == doctest::Approx(vd_rot[0]));
		CHECK(vd[1] == doctest::Approx(vd_rot[1]));
	}
}

TEST_CASE("[point] operations")
{
	using int_space = units::space<struct int_space_tag, int, 2>;
	using double_space = units::space<struct double_space_tag, double, 2>;

	int_space::point pi1{3, 4};

	SUBCASE("binary operations")
	{
		SUBCASE("addition")
		{
			constexpr int_space::vector vi{1, 6};
			constexpr int_space::point p_sum{4, 10};
			CHECK(pi1 + vi == p_sum);
			pi1 += vi;
			CHECK(pi1 == p_sum);
		}
		SUBCASE("subtraction")
		{
			constexpr int_space::point pi2{1, 6};
			constexpr int_space::vector v_diff{2, -2};
			CHECK(pi1 - pi2 == v_diff);

			constexpr int_space::vector vi{1, 6};
			constexpr int_space::point p_diff{2, -2};
			pi1 -= vi;
			CHECK(pi1 == p_diff);
		}
	}
	SUBCASE("rotations")
	{
		double_space::point pd{3.0, 4.0};
		constexpr double_space::degrees dtheta{90.0};
		constexpr double_space::point pivot{1.0, 1.0};
		constexpr double_space::point pd_rot{-2.0, 3.0};
		auto pd_rot_actual = pd.rotated(pivot, dtheta);

		CHECK(pd_rot_actual[0] == doctest::Approx(pd_rot[0]));
		CHECK(pd_rot_actual[1] == doctest::Approx(pd_rot[1]));

		pd.rotate(pivot, dtheta);

		CHECK(pd[0] == doctest::Approx(pd_rot[0]));
		CHECK(pd[1] == doctest::Approx(pd_rot[1]));
	}
}

//! @todo box tests
