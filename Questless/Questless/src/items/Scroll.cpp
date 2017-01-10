/**
* @file    Scroll.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Scroll implementation.
*/

#include "items/Scroll.h"
#include "entities/beings/Being.h"

namespace questless
{
	std::vector<Action::ptr> Scroll::actions()
	{
		std::vector<Action::ptr> actions;
		if (!blank()) {
			if (_spell->charges() > 0) {
				actions.push_back(_spell->cast());
			}
			if (auto max_charges = _spell->max_charges()) {
				if (_spell->charges() < *max_charges) {
					actions.push_back(_spell->incant());
				}
			}
			if (_spell->charges() > 0) {
				actions.push_back(_spell->discharge());
			}
		}
		actions.push_back(Drop::make(*this));
		actions.push_back(Throw::make(*this));
		return actions;
	}
}
