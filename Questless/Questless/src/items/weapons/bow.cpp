//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "bow.hpp"

#include "arrow.hpp"
#include "quiver.hpp"

#include "items/breakable.hpp"
#include "items/equipment.hpp"
#include "items/item.hpp"

namespace ql {
	void bow::nock(ent arrow_id) {
		//! @todo This.
	}
	void bow::draw() {
		//! @todo This.
	}
	void bow::loose() {
		//! @todo This.
	}

	void make_bow(ent id) {
		make_item(id, 1.5_mass);

		make_breakable(id, 400.0_integrity, 400.0_integrity);

		//! @todo Equiping and unequiping a bow shouldn't take a long time (or actions). Stringing and unstringing a bow
		//! should take a lot of time (or actions).
		make_equipment(id,
			std::nullopt,
			{equipment::tab{body_part::tag::hand, std::nullopt}, equipment::tab{body_part::tag::hand, std::nullopt}},
			12_tick, // Equip time
			3_tick); // Unequip time

		reg.assign<bow>(id, id);
	}
}
