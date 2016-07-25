/**
* @file    Craftable.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the abstract Craftable class. Items which can be crafted from other items should multiply inherit from this class and Item.
*/

#ifndef CRAFTABLE_H
#define CRAFTABLE_H

#include <vector>
#include <memory>

#include "Item.h"

namespace questless
{
	class Craftable
	{
	public:
		virtual ~Craftable() = 0 {}

		enum class Action {mix, boil, smoke, crush};
	protected:
		Action _action;
		vector<shared_ptr<Item>> _ingredients;
	};
}

#endif