/**
* @file    Spell.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Spell abstract base class.
*/

#include "spell/Spell.h"
#include "entities/beings/Being.h"
#include "agents/Agent.h"

using namespace std;

namespace questless::spell
{
	/// @todo I don't like these optional things... There has to be a better way.

	void Spell::charge(int amount)
	{
		if (boost::optional<int> max = max_charges()) {
			_charges += amount;
			if (_charges > *max) {
				_charges = *max;
			}
		}
	}

	void Spell::discharge(int amount)
	{
		if (max_charges()) {
			_charges -= amount;
			if (_charges < 0) {
				_charges = 0;
			}
		}
	}

	void Spell::update()
	{
		_active_cooldown -= 1.0;
		if (_active_cooldown < 0.0) {
			_active_cooldown = 0.0;
		}
	}
}