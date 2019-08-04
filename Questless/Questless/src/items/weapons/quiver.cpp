//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "quiver.hpp"

#include "items/equipment.hpp"
#include "items/item.hpp"

namespace ql {
	id make_quiver(id id, inventory inventory) {
		make_item(id, 0.5_mass);

		//! @todo Equiping and unequiping a bow shouldn't take a long time (or actions). Stringing and unstringing a bow
		//! should take a lot of time (or actions).
		make_equipment(id,
			std::nullopt,
			{equipment::tab{body_part::tag::torso, std::nullopt}},
			1_ap, // Equip time
			1_ap); // Unequip time

		reg.assign<quiver>(id, id, std::move(inventory));

		return id;
	}
}
