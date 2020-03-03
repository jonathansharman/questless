//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "quiver.hpp"

#include "items/equipment.hpp"
#include "items/item.hpp"

namespace ql {
	auto make_quiver(reg& reg, id quiver_id, inventory inventory) -> id {
		make_item(reg, quiver_id, 0.5_mass);

		//! @todo Equiping and unequiping a bow shouldn't take a long time (or actions). Stringing and unstringing a bow
		//! should take a lot of time (or actions).
		make_equipment( //
			reg,
			quiver_id,
			std::nullopt,
			{equipment::tab{body_part::tag::torso, std::nullopt}},
			1_ap, // Equip time
			1_ap); // Unequip time

		reg.assign<quiver>(quiver_id, &reg, quiver_id, std::move(inventory));

		return quiver_id;
	}
}
