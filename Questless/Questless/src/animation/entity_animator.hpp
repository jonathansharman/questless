//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/entity_visitor.hpp"
#include "utility/reference.hpp"

namespace ql {
	namespace rsrc {
		struct entity;
	}
	class animation;

	//! Creates animations for entities.
	struct entity_animator : public entity_const_visitor {
		rsrc::entity const& resources;

		//! The animation created by the last visit.
		uptr<animation> animation;

		entity_animator(rsrc::entity const& resources);

		~entity_animator();

		void visit(human const&) final;
		void visit(goblin const&) final;

		void visit(campfire const&) final;
		void visit(corpse const&) final;
		void visit(item_box const&) final;
	};
}
