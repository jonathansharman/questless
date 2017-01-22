/**
* @file    Potion.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Potion class.
*/

#pragma once

#include <functional>

#include "Item.h"
#include "Craftable.h"

namespace questless
{
	class Potion : public Item, public Craftable
	{
	public:
		virtual ~Potion() = 0 {}
	protected:
		function<void> _effect;
	};
}
