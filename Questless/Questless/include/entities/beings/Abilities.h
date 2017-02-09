/**
* @file    Abilities.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The list of actions a being is able to perform.
*/

#pragma once

#include <vector>
#include <memory>

#include "Action.h"

namespace questless
{
	class Being;

	class Abilities
	{
	public:
		bool can_swim() const { return _can_swim; }
		bool can_fly() const { return _can_fly; }

		void update(Being& being);
	private:
		bool _can_swim;
		bool _can_fly;
	};
}
