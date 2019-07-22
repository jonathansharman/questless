//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "quiver.hpp"

#include "items/equipment.hpp"
#include "items/item.hpp"

namespace ql {
	void make_quiver(ent id) {
		make_item(id, 0.5_mass);

		//! @todo Equiping and unequiping a bow shouldn't take a long time (or actions). Stringing and unstringing a bow
		//! should take a lot of time (or actions).
		make_equipment(id,
			std::nullopt,
			{equipment::tab{body_part::tag::torso, std::nullopt}},
			1_tick, // Equip time
			1_tick); // Unequip time

		reg.assign<quiver>(id, id, inventory{});
	}
}
