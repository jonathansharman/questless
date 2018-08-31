//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "constants.hpp"
#include "math.hpp"

#include <algorithm>
#include <array>
#include <initializer_list>
#include <numeric>
#include <ratio>
#include <type_traits>
#include <vector>

namespace units {
	namespace detail {
		template <typename Scalar, int DimensionCount>
		class buffer_base {
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
	}

	//! A generic type-safe space, parameterized over a number of dimensions and a scalar type.
	//! @tparam Tag A tag used to distinguish this space semantically from other structurally equivalent spaces.
	//! @tparam Scalar The data type of scalar values in this space.
	//! @tparam DimensionCount The number of dimensions in this space. Must be non-negative.
	//! @tparam Buffer A type containing a @p std::array<Scalar, DimensionCount>. May include space-spefific data accessors for use in points and vectors.
	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer = detail::buffer_base<Scalar, DimensionCount>>
	struct space {
	private:
		//! @todo Is there a way to generate the type for the alignment tuple without using a dummy function?

		template <int... DimensionIndices>
		static decltype(auto) align_dummy(std::integer_sequence<int, DimensionIndices...>) {
			return std::tuple<typename axis<DimensionIndices>::align...>{};
		}

		// Private constructor to prevent instantiation.
		space() = default;
	public:
		//! The scalar type of this space.
		using scalar = Scalar;

		//! The number of dimensions in this space.
		static constexpr int n = DimensionCount;

		static_assert(n >= 0, "Number of dimensions must be non-negative.");

		//! Represents the axis along the dimension with index @p Index.
		template <int Index>
		struct axis {
			static_assert(0 <= Index && Index < n, "Axis index out of bounds.");

			//! Alignment along this axis.
			enum class align { near, mid, far };
		};

		//! The alignment along every axis in this space.
		using alignment = decltype(align_dummy(std::make_integer_sequence<int, n>{}));

		//! An angle in this space, with @p UnitsPerCircle angle units per full circle, as a std::ratio.
		template <typename UnitsPerCircle>
		class angle {
		public:
			using scalar = scalar;
			using units_per_circle = UnitsPerCircle;

			//! The number of this angle unit in one full circle.
			static constexpr angle circle() { return angle{static_cast<scalar>(units_per_circle::num) / static_cast<scalar>(units_per_circle::den)}; }

			constexpr angle() = default;
			constexpr angle(angle const&) = default;
			constexpr angle(angle&&) = default;

			constexpr explicit angle(std::array<scalar, n - 1> components)
				: _components{std::move(components)}
			{}

			template <typename... Args>
			constexpr explicit angle(Args... args)
				: _components{{std::forward<Args>(args)...}} {
				static_assert(sizeof...(args) == n - 1, "All components of an angle must be initialized.");
			}

			constexpr angle& operator =(angle const&) & = default;
			constexpr angle& operator =(angle&&) & = default;

			//! The zero angle for the specified space and unit size.
			static constexpr angle zero() { return angle{scalar(0)}; }

			constexpr scalar& operator [](std::size_t index) { return _components[index]; }
			constexpr scalar operator [](std::size_t index) const { return _components[index]; }

			friend std::ostream& operator <<(std::ostream& out, angle const& phi) {
				for (angle component : phi._components) {
					out << component << ' ';
				}
				return out;
			}

			//! phi1 < phi2 iff phi1[i] < phi2[i] for all i in [0, n - 1).
			constexpr friend bool operator <(angle const& lhs, angle const& rhs) {
				for (std::size_t i = 0; i < n - 1; ++i) {
					if (lhs._components[i] >= rhs._components[i]) return false;
				}
				return true;
			}
			//! phi1 <= phi2 iff phi1[i] <= phi2[i] for all i in [0, n - 1).
			constexpr friend bool operator <=(angle const& lhs, angle const& rhs) {
				for (std::size_t i = 0; i < n - 1; ++i) {
					if (lhs._components[i] <= rhs._components[i]) return false;
				}
				return true;
			}
			constexpr friend bool operator ==(angle const& lhs, angle const& rhs) { return lhs._component == rhs._component; }
			constexpr friend bool operator !=(angle const& lhs, angle const& rhs) { return lhs._component != rhs._component; }
			//! phi1 >= phi2 iff phi1[i] >= phi2[i] for all i in [0, n - 1).
			constexpr friend bool operator >=(angle const& lhs, angle const& rhs) {
				for (std::size_t i = 0; i < n - 1; ++i) {
					if (lhs._components[i] >= rhs._components[i]) return false;
				}
				return true;
			}
			//! phi1 > phi2 iff phi1[i] > phi2[i] for all i in [0, n - 1).
			constexpr friend bool operator >(angle const& lhs, angle const& rhs) {
				for (std::size_t i = 0; i < n - 1; ++i) {
					if (lhs._components[i] > rhs._components[i]) return false;
				}
				return true;
			}

			// Closed under addition.
			constexpr angle& operator +=(angle const& that) & {
				for (std::size_t i = 0; i < n - 1; ++i) {
					this->_components[i] += that._components[i];
				}
				return *this;
			}
			constexpr friend angle operator +(angle const& lhs, angle const& rhs) {
				angle result = lhs;
				result += rhs;
				return result;
			}

			// Closed under substraction.
			constexpr angle& operator -=(angle const& that) & {
				for (std::size_t i = 0; i < n - 1; ++i) {
					this->_components[i] -= that._components[i];
				}
				return *this;
			}
			constexpr friend angle operator -(angle const& lhs, angle const& rhs) {
				angle result = lhs;
				result -= rhs;
				return result;
			}

			// Closed under negation.
			constexpr friend angle operator -(angle const& phi) {
				angle result = phi;
				for (std::size_t i = 0; i < n - 1; ++i) {
					result._components[i] = -result._components[i];
				}
				return result;
			}

			// Closed under scalar multiplication.
			constexpr angle& operator *=(scalar k) & {
				for (scalar& component : _components) {
					component = component * k;
				}
				return *this;
			}
			constexpr friend angle operator *(scalar k, angle const& phi) {
				angle result = phi;
				for (scalar& component : result._components) {
					component = k * component;
				}
				return result;
			}
			constexpr friend angle operator *(angle const& phi, scalar k) {
				angle result = phi;
				result *= k;
				return phi;
			}

			// Closed under scalar division.
			constexpr angle& operator /=(scalar k) & {
				for (scalar& component : _components) {
					component = component / k;
				}
				return *this;
			}
			constexpr friend angle operator /(angle const& phi, scalar k) {
				angle result = phi;
				result /= k;
				return result;
			}

			//! Scalar value accessor for 2D spaces, for convenience.
			template <typename = std::enable_if_t<n == 2>>
			constexpr scalar& count() { return _components[0]; }

			//! Scalar value accessor for 2D spaces, for convenience.
			template <typename = std::enable_if_t<n == 2>>
			constexpr scalar count() const { return _components[0]; }
		private:
			std::array<scalar, n - 1> _components;
		};

		//! Casts @p phi from @p FromAngle to @p ToAngle.
		template<typename ToAngle, typename FromAngle>
		static constexpr ToAngle angle_cast(FromAngle const& phi) {
			using ratio = std::ratio_divide<typename ToAngle::units_per_circle, typename FromAngle::units_per_circle>;
			using from_scalar = typename FromAngle::scalar;
			using to_scalar = typename ToAngle::scalar;

			if constexpr(std::is_same_v<std::decay_t<ToAngle>, std::decay_t<FromAngle>>) {
				return phi;
			} else {
				auto result = typename ToAngle::zero();
				for (std::size_t i = 0; i < n - 1; ++i) {
					result[i] = static_cast<to_scalar>(phi[i] * static_cast<from_scalar>(ratio::num) / static_cast<from_scalar>(ratio::den));
				}
				return result;
			}
		}

		using radians = angle<std::ratio<static_cast<int>(constants::tau * 1'000'000), 1'000'000>>;

		using degrees = angle<std::ratio<360>>;

		using gradians = angle<std::ratio<400>>;

		//! A vector in this space.
		class vector : public Buffer {
		public:
			constexpr vector() = default;
			constexpr vector(vector const&) = default;
			constexpr vector(vector&&) = default;

			constexpr explicit vector(std::array<scalar, n> components)
				: Buffer{std::move(components)}
			{}

			template <typename... Args>
			constexpr explicit vector(Args... args)
				: Buffer{{std::forward<Args>(args)...}} {
				static_assert(sizeof...(args) == n, "All components of a vector must be initialized.");
			}

			//! Constructs a vector with angle @p phi and radius @p r.
			template <typename UnitsPerCircle>
			explicit vector(typename space::angle<UnitsPerCircle> phi, scalar r) { //! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
				static_assert(n >= 2, "Vector from angle and radius requires a space with two or more dimensions.");

				std::fill(this->_elements.begin(), this->_elements.end(), r);
				for (std::size_t i = 1; i < n; ++i) {
					for (std::size_t j = 0; j < i; ++j) {
						this->_elements[i] *= std::sin(phi[j]);
					}
				}
				for (std::size_t i = 0; i < n - 1; ++i) {
					this->_elements[i] *= std::cos(phi[i]);
				}
			}

			vector& operator =(vector const&) & = default;
			vector& operator =(vector&&) & = default;

			//! The zero vector for this space.
			static constexpr vector zero() { return vector{}; }

			constexpr scalar& operator [](std::size_t index) { return this->_elements[index]; }
			constexpr scalar operator [](std::size_t index) const { return this->_elements[index]; }

			friend std::ostream& operator <<(std::ostream& out, vector const& v) {
				out << '(';
				for (std::size_t i = 0; i < v._elements.size() - 1; ++i) {
					out << v[i] << ", ";
				}
				out << v._elements.back() << ')';
				return out;
			}

			bool operator ==(vector const& that) const { return this->_elements == that._elements; }
			bool operator !=(vector const& that) const { return this->_elements != that._elements; }

			friend vector operator +(vector const& v1, vector const& v2) {
				auto result = v1;
				result += v2;
				return result;
			}

			friend vector operator -(vector const& v1, vector const& v2) {
				auto result = v1;
				result -= v2;
				return result;
			}
			friend vector operator -(vector const& v) {
				auto result = v;
				for (int i = 0; i < n; ++i) {
					result[i] = -result[i];
				}
				return result;
			}

			//! Component-wise product.
			friend vector operator *(vector const& v1, vector const& v2) {
				auto result = v1;
				for (int i = 0; i < n; ++i) {
					result[i] *= v2[i];
				}
				return result;
			}
			friend vector operator *(vector const& v, scalar k) {
				auto result = v;
				result *= k;
				return result;
			}
			friend vector operator *(scalar k, vector const& v) {
				auto result = v;
				for (int i = 0; i < n; ++i) {
					result[i] = k * result[i];
				}
				return result;
			}

			friend vector operator /(vector const& v, scalar k) {
				auto result = v;
				result /= k;
				return result;
			}

			vector& operator +=(vector const& that) & {
				for (int i = 0; i < n; ++i) {
					this->_elements[i] += that[i];
				}
				return *this;
			}
			vector& operator -=(vector const& that) & {
				for (int i = 0; i < n; ++i) {
					this->_elements[i] -= that[i];
				}
				return *this;
			}
			vector& operator *=(scalar k) & {
				for (int i = 0; i < n; ++i) {
					this->_elements[i] *= k;
				}
				return *this;
			}
			vector& operator /=(scalar k) & {
				for (int i = 0; i < n; ++i) {
					this->_elements[i] /= k;
				}
				return *this;
			}

			//! Rotates the vector, overwriting the original value.
			//! @param dphi The rotation to apply.
			//! @note @dphi will be converted to radians for this computation, so passing it as radians will save an angle conversion.
			template <typename UnitsPerCircle>
			void rotate(typename space::angle<UnitsPerCircle> const& dphi) & {
				static_assert(std::is_floating_point_v<scalar> && n == 2, "Requires two-dimensional floating-point space.");

				radians const dphi_radians = angle_cast<radians>(dphi);
				auto const cos_dphi = static_cast<scalar>(cos(dphi_radians.count()));
				auto const sin_dphi = static_cast<scalar>(sin(dphi_radians.count()));
				*this = vector
					{ static_cast<scalar>(this->_elements[0] * cos_dphi - this->_elements[1] * sin_dphi)
					, static_cast<scalar>(this->_elements[0] * sin_dphi + this->_elements[1] * cos_dphi)
					};
			}

			//! Creates a rotated copy of the vector.
			//! @param dphi The rotation to apply.
			//! @note @dphi will be converted to radians for this computation, so passing it as radians will save an angle conversion.
			template <typename UnitsPerCircle>
			vector rotated(typename space::angle<UnitsPerCircle> const& dphi) const {
				//! @todo Cannot be constexpr because of cos() and sin(). Implement constexpr trig functions in units::math to enable.
				static_assert(std::is_floating_point_v<scalar> && n == 2, "Requires two-dimensional floating-point space.");

				radians const dphi_radians = angle_cast<radians>(dphi);
				auto const cos_dphi = static_cast<scalar>(cos(dphi_radians.count()));
				auto const sin_dphi = static_cast<scalar>(sin(dphi_radians.count()));
				return vector
					{ static_cast<scalar>(this->_elements[0] * cos_dphi - this->_elements[1] * sin_dphi)
					, static_cast<scalar>(this->_elements[0] * sin_dphi + this->_elements[1] * cos_dphi)
					};
			}

			//! The vector's length.
			constexpr scalar length() const {
				scalar sum_of_squares(0);
				for (int i = 0; i < n; ++i) {
					sum_of_squares += this->_elements[i] * this->_elements[i];
				}
				return static_cast<scalar>(math::sqrt(sum_of_squares));
			}

			//! The square of the vector's length.
			constexpr scalar length_squared() const {
				scalar result(0);
				for (int i = 0; i < n; ++i) {
					result += this->_elements[i] * this->_elements[i];
				}
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

		//! A point in this space.
		class point : public Buffer {
		public:
			constexpr point() = default;
			constexpr point(point const&) = default;
			constexpr point(point&&) = default;

			constexpr explicit point(std::array<scalar, n> coordinates)
				: Buffer{std::move(coordinates)}
			{}

			template <typename... Args>
			constexpr explicit point(Args... args) : Buffer{{std::forward<Args>(args)...}} {
				static_assert(sizeof...(args) == n, "All coordinates of a point must be initialized.");
			}

			point& operator =(point const&) & = default;
			point& operator =(point&&) & = default;

			constexpr scalar& operator [](std::size_t index) { return this->_elements[index]; }
			constexpr scalar operator [](std::size_t index) const { return this->_elements[index]; }

			friend std::ostream& operator <<(std::ostream& out, point const& p) {
				out << '(';
				for (std::size_t i = 0; i < n - 1; ++i) {
					out << p[i] << ", ";
				}
				out << p[n - 1] << ')';
				return out;
			}

			constexpr bool operator ==(point const& that) const {
				for (int i = 0; i < n; ++i) {
					if (this->_elements[i] != that[i]) {
						return false;
					}
				}
				return true;
			}
			constexpr bool operator !=(point const& that) const {
				for (int i = 0; i < n; ++i) {
					if (this->_elements[i] != that[i]) {
						return true;
					}
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

			//! Component-wise product.
			friend point operator *(point const& p, vector const& v) {
				auto result = p;
				result *= v;
				return result;
			}
			//! Component-wise product.
			friend point operator *(vector const& v, point const& p) {
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
		};

		//! A rectangular, orthogonal volume in this space.
		class box {
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

			friend std::ostream& operator <<(std::ostream& out, box const& box) {
				out << '(' << box.origin << ", " << box.size << ')';
				return out;
			}

			constexpr friend bool operator ==(box const& left, box const& right) {
				return left.origin == right.origin && left.size == right.size;
			}
			constexpr friend bool operator !=(box const& left, box const& right) {
				return left.origin != right.origin || left.size != right.size;
			}

			//! Whether this box contains @p point.
			constexpr bool contains(point const& point) const {
				//! @todo Rigorously define what containment means for generic spaces (inclusive vs. exclusive bounds).
				for (int i = 0; i < n; ++i) {
					if (point[i] < position[i] || point[i] > position[i] + size[i]) {
						return false;
					}
				}
				return true;
			}

			//! Translates this box by the given offset.
			void translate(vector const& offset) & {
				position += offset;
			}

			//! A copy of this box translated by @p offset.
			constexpr box translated(vector const& offset) const {
				return box{position + offset, size};
			}

			//! Extends this box, if necessary, to include the given point.
			void extend(point const& point) {
				for (int i = 0; i < n; ++i) {
					if (point[i] < position[i]) {
						size[i] += position[i] - point[i];
						position[i] = point[i];
					} else if (point[i] > position[i] + size[i]) {
						size[i] = point[i] - position[i];
					}
				}
			}
		private:
			static constexpr point aligned_position(point position, vector size, alignment const& alignment) {
				return apply_alignment<0>(position, size, alignment);
			}

			template <int Index>
			static constexpr point apply_alignment(point position, vector size, alignment const& alignment) {
				//! @todo Specialize these adjustments for integers and floats?
				switch (std::get<Index>(alignment)) {
					case axis<Index>::align::near:
						break;
					case axis<Index>::align::mid:
						position[Index] -= size[Index] / 2 - 1;
						break;
					case axis<Index>::align::far:
						position[Index] -= size[Index] - 1;
						break;
				}
				if constexpr (Index == n - 1) {
					return position;
				} else {
					return apply_alignment<Index + 1>(position, size, alignment);
				}
			}
		};

		//! A sphere in this space. E.g. a circle in 2D spaces or a 
		class sphere {
		public:
			//! The center point of this sphere.
			point center;

			//! The radius of this sphere.
			scalar radius;

			constexpr sphere() = default;
			constexpr sphere(sphere const&) = default;
			constexpr sphere(sphere&&) = default;

			//! A sphere centered at @p center with radius @p radius.
			explicit constexpr sphere(point center, scalar radius)
				: center{std::move(center)}
				, radius{std::move(radius)}
			{}

			sphere& operator =(sphere const&) & = default;
			sphere& operator =(sphere&&) & = default;

			friend std::ostream& operator <<(std::ostream& out, sphere const& sphere) {
				out << '(' << sphere.center << ", " << box.radius << ')';
				return out;
			}

			constexpr friend bool operator ==(sphere const& left, sphere const& right) {
				return left.center == right.center && left.radius == right.radius;
			}
			constexpr friend bool operator !=(sphere const& left, sphere const& right) {
				return left.center != right.center || left.radius != right.radius;
			}

			//! Whether this sphere contains @p point.
			constexpr bool contains(point const& point) const {
				//! @todo Rigorously define what containment means for generic spaces (inclusive vs. exclusive bounds).
				return (center - point).length_squared() <= radius * radius;
			}

			//! Translates this sphere by the given offset.
			void translate(vector const& offset) & {
				center += offset;
			}

			//! A copy of this sphere translated by @p offset.
			constexpr sphere translated(vector const& offset) const {
				return sphere{center + offset, radius};
			}

			//! Extends this sphere, if necessary, to include the given point.
			void extend(point const& point) {
				if (!contains(point)) {
					radius = (center - point).length();
				}
			}
		};

		//! A simple closed polygon in this space defined by a sequence of vertices. Only defined for 2D spaces.
		//! @note Behavior is undefined if the given points do not form a simple closed polytope.
		using polygon = std::vector<point>;

		//! The smallest box that contains @p polygon.
		static box bounding_box(polygon const& polygon) {
			static_assert(n == 2, "Use of polygon operations requires a 2D type.");

			//! @todo Check that the edge cases are correct here. Calling .contains(v) on the result of this function should work for all v in polygon.
			auto min_x = std::accumulate(polygon.begin(), polygon.end(), std::numeric_limits<scalar>::max(), [](scalar const& acc, point const& v) {
				return std::min(acc, v.x());
			});
			auto max_x = std::accumulate(polygon.begin(), polygon.end(), std::numeric_limits<scalar>::min(), [](scalar const& acc, point const& v) {
				return std::max(acc, v.x());
			});
			auto min_y = std::accumulate(polygon.begin(), polygon.end(), std::numeric_limits<scalar>::max(), [](scalar const& acc, point const& v) {
				return std::min(acc, v.y());
			});
			auto max_y = std::accumulate(polygon.begin(), polygon.end(), std::numeric_limits<scalar>::min(), [](scalar const& acc, point const& v) {
				return std::max(acc, v.y());
			});
			return box{point{min_x, min_y}, vector{max_x - min_x, max_y - min_y}};
		}

		//! Whether @p polygon contains @p point.
		static bool contains(polygon const& polygon, point const& point) {
			static_assert(n == 2, "Use of polygon operations requires a 2D type.");

			//! @todo Rigorously define what containment means for generic spaces (inclusive vs. exclusive bounds).

			// First check if the point is outside the bounding box.
			if (!bounding_box(polygon).contains(point)) { return false; }

			// Now check how many edges a ray from the point to the right intersects.
			// Adapted from https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon.
			bool result = false;
			for (std::size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
				bool const between_y_coords = polygon[i].y() > point.y() != polygon[j].y() > point.y();
				bool const cond2 = point.x() < (polygon[j].x() - polygon[i].x()) * (point.y() - polygon[i].y()) / (polygon[j].y() - polygon[i].y()) + polygon[i].x();
				if (between_y_coords && cond2) {
					result = !result;
				}
			}
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

TEST_CASE("[angle] operations") {
	using double_space = units::space<struct double_space_tag, double, 2>;

	constexpr auto circle_degrees = double_space::angle_cast<double_space::degrees>(double_space::radians::circle());

	SUBCASE("conversions") {
		CHECK(circle_degrees[0] == doctest::Approx(360.0));
	}
}

TEST_CASE("[vector] operations") {
	using int_space = units::space<struct int_space_tag, int, 2>;
	using double_space = units::space<struct double_space_tag, double, 2>;

	typename int_space::vector vi1{3, 4};

	SUBCASE("length") {
		CHECK(vi1.length() == 5);
		CHECK(vi1.length_squared() == 25);
	}
	SUBCASE("angle") {
		constexpr double_space::vector vd{3.0, 4.0};
		CHECK(vd.angle<double_space::radians>()[0] == doctest::Approx(0.927295218));
	}
	SUBCASE("binary operations") {
		constexpr int_space::vector vi2{1, 6};
		SUBCASE("addition") {
			constexpr int_space::vector v_sum{4, 10};
			CHECK(vi1 + vi2 == v_sum);
			vi1 += vi2;
			CHECK(vi1 == v_sum);
		}
		SUBCASE("subtraction") {
			constexpr int_space::vector v_diff{2, -2};
			CHECK(vi1 - vi2 == v_diff);
			vi1 -= vi2;
			CHECK(vi1 == v_diff);
		}
	}
	SUBCASE("scalar operations") {
		constexpr int k = 2;
		SUBCASE("scalar multiplication") {
			constexpr int_space::vector v_prod{6, 8};
			CHECK(k * vi1 == v_prod);
			CHECK(vi1 * k == v_prod);
			vi1 *= k;
			CHECK(vi1 == v_prod);
		}
		SUBCASE("scalar division") {
			constexpr int_space::vector v_quot{1, 2};
			CHECK(vi1 / k == v_quot);
			vi1 /= k;
			CHECK(vi1 == v_quot);
		}
	}
	SUBCASE("rotations") {
		typename double_space::vector vd{3.0, 4.0};
		constexpr typename double_space::degrees dphi{90.0};
		constexpr typename double_space::vector vd_rot{-4.0, 3.0};
		auto vd_rot_actual = vd.rotated(dphi);

		CHECK(vd_rot_actual[0] == doctest::Approx(vd_rot[0]));
		CHECK(vd_rot_actual[1] == doctest::Approx(vd_rot[1]));

		vd.rotate(dphi);

		CHECK(vd[0] == doctest::Approx(vd_rot[0]));
		CHECK(vd[1] == doctest::Approx(vd_rot[1]));
	}
}

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

//! @todo box tests

//! @todo sphere tests

TEST_CASE("[polygon] operations") {
	//! @todo Test integral spaces.

	//! @todo Why don't these tests compile?

	//struct double_space : units::space<struct double_space_tag, double, 2> {};

	//double_space::point v1{0.0, 0.0};
	//double_space::point v2{1.0, 0.0};
	//double_space::point v3{0.0, 1.0};
	//double_space::polygon poly{v1, v2, v3};

	//SUBCASE("bounding box")
	//{
	//	double_space::box const bounding_box
	//		{ double_space::point{0.0, 1.0}
	//		, double_space::vector{1.0, 1.0}
	//		};
	//	CHECK(poly.bounding_box() == bounding_box);
	//}
}
