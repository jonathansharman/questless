//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/Item.h"
#include "utility/DynamicProperty.h"

namespace questless
{
	//! Abstract base class for items which can be worn down and broken.
	class Breakable : public virtual Item
	{
	public:
		virtual ~Breakable() = default;

		//! The item's durability, which is its maximum integrity.
		virtual double durability() const = 0;

		//! Whether the item is broken, i.e., integrity is zero.
		bool broken() const { return integrity <= 0.0; }

		//! The item's integrity, which ranges from zero to its durability. The item is broken if integrity is zero.
		DynamicProperty<double> integrity;
	protected:
		Breakable(double integrity) : integrity{integrity}
		{
			// Can't perform range checking yet because call to durability() will fail until the object is fully constructed.
			this->integrity.set_mutator(integrity_mutator(), false);
		}
	private:
		std::function<void(double&, double const&)> integrity_mutator();
	};
}
