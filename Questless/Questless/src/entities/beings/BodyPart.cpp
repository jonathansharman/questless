/**
* @file    BodyPart.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the BodyPart class.
*/

#include "entities/beings/BodyPart.h"

using std::string;
using std::vector;

using namespace sdl;

namespace questless
{
	BodyPart::BodyPart(string name, double vitality, Attributes attributes, bool vital, vector<Rect> regions)
		: _name{std::move(name)}
		, _attributes{attributes}
		, _vital{vital}
		, _regions{std::move(regions)}
		, _health{vitality}
		, _vitality{vitality}
	{
		for (auto& region : _regions) {
			region.x *= 5;
			region.y *= 5;

			region.y = -region.y;

			region.w *= 5;
			region.h *= 5;

			++region.w;
			++region.h;
		}
	}
}
