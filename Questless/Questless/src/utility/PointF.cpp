/**
* @file    PointF.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for PointF.
*/

#include "utility/PointF.h"

namespace questless
{
	PointF operator +(PointF p, const VectorF& v)
	{
		p.x += v.x;
		p.y += v.y;
		return p;
	}

	PointF operator +(const VectorF& v, PointF p)
	{
		p.x += v.x;
		p.y += v.y;
		return p;
	}

	PointF operator -(PointF p, const VectorF& v)
	{
		p.x -= v.x;
		p.y -= v.y;
		return p;
	}

	VectorF operator -(const PointF& p1, const PointF& p2)
	{
		return VectorF(p1.x - p2.x, p1.y - p2.y);
	}

	PointF operator *(PointF p, double factor)
	{
		p.x *= factor;
		p.y *= factor;
		return p;
	}

	PointF operator *(double factor, PointF p)
	{
		p.x *= factor;
		p.y *= factor;
		return p;
	}

	PointF operator /(PointF p, double divisor)
	{
		p.x /= divisor;
		p.y /= divisor;
		return p;
	}
	
	PointF& PointF::operator +=(const VectorF& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	PointF& PointF::operator -=(const VectorF& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	PointF& PointF::operator *=(double factor)
	{
		x *= factor;
		y *= factor;
		return *this;
	}

	PointF& PointF::operator /=(double divisor)
	{
		x /= divisor;
		y /= divisor;
		return *this;
	}

	void PointF::rotate(const PointF& origin, double theta)
	{
		double cos_theta = cos(tau * -theta / 360);
		double sin_theta = sin(tau * -theta / 360);
		VectorF offset = VectorF(origin.x, origin.y);
		*this -= offset;
		*this = PointF(x * cos_theta - y * sin_theta, x * sin_theta + y * cos_theta) + offset;
	}

	PointF PointF::rotated(const PointF& origin, double theta) const
	{
		double cos_theta = cos(tau * -theta / 360);
		double sin_theta = sin(tau * -theta / 360);
		VectorF offset = VectorF(origin.x, origin.y);
		PointF temp = *this - offset;
		return PointF(temp.x * cos_theta - temp.y * sin_theta, temp.x * sin_theta + temp.y * cos_theta) + offset;
	}

	Point PointF::to_point() const
	{
		return Point(lround(x), lround(y));
	}
}