/**
* @file    Velocity.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Velocity class.
*/

#include "utility/Velocity.h"

namespace questless
{
	Velocity operator +(const Velocity& v1, const Velocity& v2)
	{
		return Velocity{v1.x + v2.x, v1.y + v2.y};
	}

	Velocity operator -(const Velocity& v1, const Velocity& v2)
	{
		return Velocity{v1.x - v2.x, v1.y - v2.y};
	}

	/*
	VectorF operator *(const Velocity& velocity, duration<double> duration)
	{
		return VectorF{velocity.x * duration, velocity.y * duration};
	}

	VectorF operator *(duration<double> duration, const Velocity& velocity)
	{
		return VectorF{velocity.x * duration, velocity.y * duration};
	}
	*/

	Velocity operator *(const Velocity& v, double factor)
	{
		return Velocity{v.x * factor, v.y * factor};
	}

	Velocity operator *(double factor, const Velocity& v)
	{
		return Velocity{v.x * factor, v.y * factor};
	}

	Velocity operator /(const VectorF& displacement, double_seconds duration)
	{
		return Velocity{displacement, duration};
	}

	Velocity operator /(const Velocity& v, double divisor)
	{
		return Velocity{v.x / divisor, v.y / divisor};
	}
		
	Velocity& Velocity::operator +=(const Velocity& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Velocity& Velocity::operator -=(const Velocity& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Velocity& Velocity::operator *=(double factor)
	{
		x *= factor;
		y *= factor;
		return *this;
	}

	Velocity& Velocity::operator /=(double divisor)
	{
		x /= divisor;
		y /= divisor;
		return *this;
	}

	void Velocity::rotate(double theta)
	{
		double cos_theta = cos(tau * -theta / 360);
		double sin_theta = sin(tau * -theta / 360);
		*this = Velocity{x * cos_theta - y * sin_theta, x * sin_theta + y * cos_theta};
	}

	Velocity Velocity::rotated(double theta) const
	{
		double cos_theta = cos(tau * -theta / 360);
		double sin_theta = sin(tau * -theta / 360);
		return Velocity{this->x * cos_theta - this->y * sin_theta, this->x * sin_theta + this->y * cos_theta};
	}
}