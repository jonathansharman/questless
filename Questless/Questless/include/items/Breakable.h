/**
* @file    Breakable.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Breakable abstract base class, for items which can be worn down and broken.
*/

#pragma once

#include "items/Item.h"

namespace questless
{
	class Breakable : public virtual Item
	{
	public:
		virtual ~Breakable() = default;

		/// @return The item's durability, which is its maximum integrity.
		virtual double durability() const = 0;

		/// @return Whether the item is broken, i.e., integrity is zero.
		bool broken() const { return _integrity <= 0.0; }

		/// @return The item's integrity, which ranges from zero to durability. The item is broken if integrity is zero.
		double integrity() const { return _integrity; }

		/// Sets integrity to the given value.
		/// @param value The new value for integrity. The value is clamped to [0, durability].
		void integrity(double value)
		{
			_integrity = value;
			if (_integrity < 0.0) { _integrity = 0.0; }
			if (_integrity > durability()) { _integrity = durability(); }
		}

		/// Reduces the item's integrity by the given amount. Integrity cannot deceed zero.
		/// @param amount The amount of wear. No effect if amount is non-positive.
		void wear(double amount)
		{
			if (amount <= 0.0) return;
			_integrity -= amount;
			if (_integrity < 0.0) { _integrity = 0.0; }
		}

		/// Increases the item's integrity by the given amount. Integrity cannot exceed durability.
		/// @param amount The amount of repair. No effect if amount is non-positive.
		void repair(double amount)
		{
			if (amount <= 0.0) return;
			_integrity += amount;
			if (_integrity > durability()) { _integrity = durability(); }
		}
	protected:
		Breakable(double integrity) : _integrity{integrity} {}
	private:
		double _integrity;
	};
}
