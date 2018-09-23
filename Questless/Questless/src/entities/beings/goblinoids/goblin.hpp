//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/being.hpp"

namespace ql {
	class goblin : public being_base<goblin> {
	public:
		goblin(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id = ql::id<being>::make());

		ql::entity_subtype entity_subtype() const final { return entity_subtype::goblin_class; }

		double transparency() const final { return 0.5; }

		std::string const& description() const {
			return "The goblins are a diminutive and simpleminded humanoid race, known particularly for their bad "
				"tempers and love of mischief. Goblins rarely form large communities, preferring small family groups "
				"or solitude. They are remarkably sneaky, and many an unwary traveller through goblin territory has "
				"lost a coin purse or a limb to a goblin ambush.";
		}
	protected:
		ql::body make_body(ql::id<being> owner_id) final;
	};
}
