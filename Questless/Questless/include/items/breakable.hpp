//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/item.hpp"
#include "utility/lazy_bounded.hpp"

namespace ql {
	//! Abstract base class for items which can be worn down and broken.
	class breakable : public virtual item {
	public:
		virtual ~breakable() = default;

		//! The item's durability, which is its maximum integrity.
		virtual double durability() const = 0;

		//! Whether the item is broken, i.e., integrity is zero.
		bool broken() const { return integrity <= 0.0; }

		//! The item's integrity, which ranges from zero to its durability. The item is broken if integrity is zero.
		lazy_bounded<double> integrity;
	protected:
		breakable(double integrity)
			: integrity{integrity, [] { return 0.0; }, [this] { return durability(); }}
		{}
	private:
		std::function<void(double&, double const&)> integrity_mutator();
	};

	DEFINE_ELEMENT_BASE_MAKE_CTOR(breakable, item)
}
