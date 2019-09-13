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
	auto bow::nock(ql::id /*arrow_id*/) -> void {
		//! @todo This.
	}
	auto bow::draw() -> void {
		//! @todo This.
	}
	auto bow::loose() -> void {
		//! @todo This.
	}

	auto make_bow(id bow_id) -> id {
		make_item(bow_id, 1.5_mass);

		make_breakable(bow_id, {400_durability, 400_durability});

		//! @todo Equiping and unequiping a bow shouldn't take a long time (or actions). Stringing and unstringing a bow
		//! should take a lot of time (or actions).
		make_equipment(bow_id,
			std::nullopt,
			{equipment::tab{body_part::tag::hand, std::nullopt}, equipment::tab{body_part::tag::hand, std::nullopt}},
			12_ap,
			3_ap);

		reg.assign<bow>(bow_id, bow_id);

		return bow_id;
	}
}
