//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/item_visitor.hpp"
#include "utility/reference.hpp"

namespace ql {
	namespace rsrc {
		struct item;
		struct spell;
	}
	class animation;

	//! Creates animations for items.
	struct item_animator : public item_const_visitor {
		rsrc::item const& item_resources;
		rsrc::spell const& spell_resources;

		//! The animation created by the last visit.
		uptr<animation> animation = nullptr;

		~item_animator();

		void visit(arrow const&) final;
		void visit(bow const&) final;
		void visit(quarterstaff const&) final;
		void visit(quiver const&) final;
		void visit(scroll const&) final;
		void visit(gatestone const&) final;
	};
}
