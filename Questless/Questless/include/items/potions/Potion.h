/**
* @file    Potion.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Potion class.
*/

#ifndef POTION_H
#define POTION_H

#include <vector>
using std::vector;
#include <functional>
using std::function;
#include <memory>
using std::shared_ptr;

#include "constants.h"
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

#endif