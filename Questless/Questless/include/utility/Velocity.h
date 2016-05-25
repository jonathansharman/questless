/**
* @file    Velocity.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for Velocity, a struct for expressesing change in position per second. Works with Frequency, duration, and VectorF types.
*/

#ifndef VELOCITY_H
#define VELOCITY_H

#include "VectorF.h"
#include "utility.h"

namespace questless
{
	class Velocity
	{
	public:
		Hertz x; ///< Horizontal displacement per second.
		Hertz y; ///< Vertical displacement per second.

		/// Constructs a velocity with the given x and y displacements per second.
		/// @param x The horizontal displacement per second.
		/// @param y The vertical displacement per second.
		Velocity(double x, double y) : x{x}, y{y} {}

		/// Constructs a velocity with the given x and y displacements per second.
		/// @param x The horizontal displacement per second.
		/// @param y The vertical displacement per second.
		Velocity(Hertz x, Hertz y) : x{x}, y{y} {}

		/// Constructs a velocity from the given displacement per second.
		/// @param per_second A vector expressing displacement per second.
		Velocity(const VectorF& per_second) : x{per_second.x}, y{per_second.y} {}

		/// Constructs a velocity from the given unit displacement and duration.
		/// @param displacement A displacement vector.
		/// @param duration The duration over which the unit displacement occurs.
		Velocity(const VectorF& displacement, double_seconds duration)
			: x{displacement.x / duration.count()}
			, y{displacement.y / duration.count()}
		{}

		bool operator ==(const Velocity& right) const { return x == right.x && y == right.y; }
		bool operator !=(const Velocity& right) const { return x != right.x || y != right.y; }
		
		friend Velocity operator +(const Velocity& v1, const Velocity& v2);
		friend Velocity operator -(const Velocity& v1, const Velocity& v2);
		//friend VectorF operator *(const Velocity& velocity, duration<double> duration); /// @todo Reenable when MS fixes the bug with duration *
		//friend VectorF operator *(duration<double> duration, const Velocity& velocity);
		friend Velocity operator *(const Velocity& v, double factor);
		friend Velocity operator *(double factor, const Velocity& v);
		friend Velocity operator /(const VectorF& displacement, double_seconds duration);
		friend Velocity operator /(const Velocity& v, double divisor);
		
		Velocity& operator +=(const Velocity& v);
		Velocity& operator -=(const Velocity& v);
		Velocity& operator *=(double factor);
		Velocity& operator /=(double divisor);

		/// Rotates the velocity, overwriting the original value.
		/// @param theta The counter-clockwise rotation to apply, in degrees.
		void rotate(double theta);

		/// Creates a rotated copy of the velocity.
		/// @param theta The counter-clockwise rotation to apply, in degrees.
		Velocity rotated(double theta) const;

		/// @return The displacement per second as a VectorF.
		inline VectorF displacement() const { return VectorF(x.count(), y.count()); }

		/// @todo Remove unary displacement method once the appropriate * operators are fixed.

		/// Calculates the displacement that will occur after the given duration.
		/// @param duration A duration in seconds.
		/// @return The displacement.
		inline VectorF displacement(const double_seconds& duration) const { return VectorF{x * duration, y * duration}; }

		/// @return The velocity's magnitude.
		Hertz speed() const { return Hertz{sqrt(x.count() * x.count() + y.count() * y.count())}; }

		/// @return The angle of the vector in degrees counter-clockwise from the positive x-axis.
		double angle() const { return 360.0 * atan2(y.count(), x.count()) / tau; }
	};
}

#endif