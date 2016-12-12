/**
* @file    Velocity.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A type for expressesing change in position per second. Works with Frequency, duration, and VectorF types.
*/

#ifndef VELOCITY_H
#define VELOCITY_H

#include "VectorF.h"
#include "utility.h"
#include "TaggedType.h"

namespace questless
{
	using AngleRadians = TaggedType<double, struct AngleTag>;
	using Speed = TaggedType<double, struct SpeedTag>;

	constexpr Argument<AngleRadians> ANGLE_RADIANS;
	constexpr Argument<Speed> SPEED;

	struct Velocity
	{
		Hertz x; ///< Horizontal displacement per second.
		Hertz y; ///< Vertical displacement per second.

		/// Constructs a velocity with the given x and y displacements per second.
		/// @param x The horizontal displacement per second.
		/// @param y The vertical displacement per second.
		Velocity(Hertz x, Hertz y) : x{x}, y{y} {}

		/// Constructs a velocity from the given displacement per second.
		/// @param displacement_rate A vector rate expressing displacement per second.
		Velocity(const Frequency<VectorF>& displacement_rate)
			: x{displacement_rate.count().x}
			, y{displacement_rate.count().y}
		{}

		/// Constructs a velocity from the given displacement and duration.
		/// @param displacement A displacement vector.
		/// @param duration The duration over which the displacement occurs.
		Velocity(const VectorF& displacement, seconds_f duration)
			: x{displacement.x / duration.count()}
			, y{displacement.y / duration.count()}
		{}

		/// Constructs a velocity from the given angle and speed.
		/// @param angle The angle of the heading in radians, increasing CCW from the positive x-axis.
		/// @param speed The magnitude of the velocity.
		Velocity(AngleRadians angle, Speed speed)
			: x{cos(angle) * speed}
			, y{sin(angle) * speed}
		{}

		bool operator ==(const Velocity& right) const { return x == right.x && y == right.y; }
		bool operator !=(const Velocity& right) const { return x != right.x || y != right.y; }
		
		friend Velocity operator +(const Velocity& v1, const Velocity& v2);
		friend Velocity operator -(const Velocity& v1, const Velocity& v2);
		//friend VectorF operator *(const Velocity& velocity, seconds_f duration); /// @todo Reenable when MS fixes the bug with duration *
		//friend VectorF operator *(seconds_f duration, const Velocity& velocity); /// @todo Reenable when MS fixes the bug with duration *
		friend Velocity operator *(const Velocity& v, double factor);
		friend Velocity operator *(double factor, const Velocity& v);
		friend Velocity operator /(const VectorF& displacement, seconds_f duration);
		friend Velocity operator /(const Velocity& v, double divisor);

		Velocity& operator =(const Velocity& v) & = default;

		Velocity& operator +=(const Velocity& v) &;
		Velocity& operator -=(const Velocity& v) &;
		Velocity& operator *=(double factor) &;
		Velocity& operator /=(double divisor) &;

		/// Rotates the velocity, overwriting the original value.
		/// @param angle The counter-clockwise rotation to apply, in degrees.
		void rotate(double angle);

		/// Creates a rotated copy of the velocity.
		/// @param angle The counter-clockwise rotation to apply, in degrees.
		Velocity rotated(double angle) const;

		/// @return The displacement per second as a VectorF.
		inline VectorF displacement() const { return VectorF(x.count(), y.count()); }

		/// @todo Remove unary displacement method once the appropriate * operators are fixed.

		/// Calculates the displacement that will occur after the given duration.
		/// @param duration A duration in seconds.
		/// @return The displacement.
		inline VectorF displacement(const seconds_f& duration) const { return VectorF{x * duration, y * duration}; }

		/// @return The velocity's magnitude.
		Hertz speed() const { return Hertz{sqrt(x.count() * x.count() + y.count() * y.count())}; }

		/// @return The angle of the vector in degrees counter-clockwise from the positive x-axis.
		double angle() const { return 360.0 * atan2(y.count(), x.count()) / tau; }
	};
}

#endif