//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation.hpp"
#include "entities/entity_visitor.hpp"
#include "utility/reference.hpp"

namespace ql
{
	//! Creates animations for entities.
	class entity_animator : public entity_const_visitor
	{
	public:
		entity_animator();
		~entity_animator();

		void visit(human const&) final;
		void visit(goblin const&) final;

		void visit(campfire const&) final;
		void visit(corpse const&) final;
		void visit(item_box const&) final;

		//! The animation produced by the last visit. Moves the animation out of the animator.
		uptr<animation> animation() { return std::move(_animation); }
	private:
		uptr<ql::animation> _animation;
	};
}
