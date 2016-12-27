/**
* @file    Vector.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for Vector, a struct template that holds a 2D vector, with given length and space types.
*/

#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

#include "units/geometry.h"

template <typename LengthType, typename SpaceType>
struct Vector
{
	using length_t = LengthType;
	using space_t = SpaceType;

	length_t x;
	length_t y;

	constexpr explicit Vector() : x{length_t{}}, y{length_t{}} {}

	constexpr explicit Vector(length_t x, length_t y) : x{std::move(x)}, y{std::move(y)} {}

	explicit Vector(questless::AngleRadians theta, length_t r)  // Cannot be constexpr because of cos() and sin().
		: x{static_cast<length_t>(r * cos(theta))}
		, y{static_cast<length_t>(r * sin(theta))}
	{}

	constexpr Vector(const Vector& v) : x{v.x}, y{v.y} {}

	bool operator ==(const Vector& right) const { return x == right.x && y == right.y; }
	bool operator !=(const Vector& right) const { return x != right.x || y != right.y; }

	friend Vector operator +(const Vector& v1, const Vector& v2) { return Vector{v1.x + v2.x, v1.y + v2.y}; }
	friend Vector operator -(const Vector& v1, const Vector& v2) { return Vector{v1.x - v2.x, v1.y - v2.y}; }
	friend Vector operator -(const Vector& v) { return Vector{-v.x, -v.y}; }
	friend Vector operator *(Vector v, LengthType factor) { return Vector{factor * v.x, factor * v.y}; }
	friend Vector operator *(LengthType factor, Vector v) { return Vector{factor * v.x, factor * v.y}; }
	friend Vector operator /(Vector v, LengthType divisor) { return Vector{v.x / divisor, v.y / divisor}; }

	Vector& operator =(const Vector& v) & = default;

	Vector& operator +=(const Vector& v) &
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	Vector& operator -=(const Vector& v) &
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Vector& operator *=(LengthType factor) &
	{
		x *= factor;
		y *= factor;
		return *this;
	}
	Vector& operator /=(LengthType divisor) &
	{
		x /= divisor;
		y /= divisor;
		return *this;
	}

	/// Rotates the vector, overwriting the original value.
	/// @param dtheta The counter-clockwise rotation to apply, in radians.
	void rotate(questless::AngleRadians dtheta)
	{
		auto cos_dtheta = static_cast<LengthType>(cos(dtheta));
		auto sin_dtheta = static_cast<LengthType>(sin(dtheta));
		*this = Vector{x * cos_dtheta - y * sin_dtheta, x * sin_dtheta + y * cos_dtheta};
	}

	/// Creates a rotated copy of the vector.
	/// @param dtheta The counter-clockwise rotation to apply, in radians.
	Vector rotated(questless::AngleRadians dtheta) const
	{
		auto cos_dtheta = static_cast<LengthType>(cos(dtheta));
		auto sin_dtheta = static_cast<LengthType>(sin(dtheta));
		return Vector{this->x * cos_dtheta - this->y * sin_dtheta, this->x * sin_dtheta + this->y * cos_dtheta};
	}

	/// @return The vector's length.
	length_t length() const { return static_cast<length_t>(sqrt(x * x + y * y)); }

	/// @return The square of the vector's length.
	length_t length_squared() const { return static_cast<length_t>(x * x + y * y); }

	/// @return The counter-clockwise angle of the vector from the positive x-axis, in radians.
	questless::AngleRadians angle() const { return questless::AngleRadians{atan2(y, x)}; }
};

// Space Definitions

struct TextureSpace {};
struct SpriteSheetSpace {};
struct ScreenSpace {};
struct GameSpace {};

// Vector Type Definitions

using TextureVector = Vector<int, TextureSpace>;
using SpriteSheetVector = Vector<int, SpriteSheetSpace>;
using ScreenVector = Vector<int, ScreenSpace>;
using GameVector = Vector<double, GameSpace>;

#endif
