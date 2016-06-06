/**
* @file    PointF.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for PointF.
*/

#include "utility/PointF.h"

using sdl::Point;

namespace questless
{
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