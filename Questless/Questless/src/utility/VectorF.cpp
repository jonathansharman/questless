/**
* @file    PointF.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for VectorF.
*/

#include <cmath>

#include "utility/VectorF.h"

namespace questless
{
	VectorF operator +(const VectorF& v1, const VectorF& v2)
	{
		return VectorF(v1.x + v2.x, v1.y + v2.y);
	}

	VectorF operator -(const VectorF& v1, const VectorF& v2)
	{
		return VectorF(v1.x - v2.x, v1.y - v2.y);
	}

	VectorF operator *(VectorF v, double factor)
	{
		v.x *= factor;
		v.y *= factor;
		return v;
	}

	VectorF operator *(double factor, VectorF v)
	{
		v.x *= factor;
		v.y *= factor;
		return v;
	}

	VectorF operator /(VectorF v, double divisor)
	{
		v.x /= divisor;
		v.y /= divisor;
		return v;
	}

	VectorF& VectorF::operator =(const VectorF& v)
	{
		this->x = v.x;
		this->y = v.y;
		return *this;
	}
	
	VectorF& VectorF::operator +=(const VectorF& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	VectorF& VectorF::operator -=(const VectorF& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	VectorF& VectorF::operator *=(double factor)
	{
		x *= factor;
		y *= factor;
		return *this;
	}

	VectorF& VectorF::operator /=(double divisor)
	{
		x /= divisor;
		y /= divisor;
		return *this;
	}

	void VectorF::rotate(double theta)
	{
		double cos_theta = cos(tau * -theta / 360);
		double sin_theta = sin(tau * -theta / 360);
		*this = VectorF{x * cos_theta - y * sin_theta, x * sin_theta + y * cos_theta};
	}

	VectorF VectorF::rotated(double theta) const
	{
		double cos_theta = cos(tau * -theta / 360);
		double sin_theta = sin(tau * -theta / 360);
		return VectorF{this->x * cos_theta - this->y * sin_theta, this->x * sin_theta + this->y * cos_theta};
	}

	Vector VectorF:: to_vector() const
	{
		return Vector{lround(x), lround(y)};
	}
}