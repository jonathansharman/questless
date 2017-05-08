//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Animation.h"
#include "entities/EntityVisitor.h"
#include "sdl/resources.h"
#include "utility/reference.h"

namespace questless
{
	//! Creates animations for entities.
	class EntityAnimator : public EntityConstVisitor
	{
	public:
		void visit(Human const&) final;
		void visit(Goblin const&) final;

		void visit(Campfire const&) final;
		void visit(Corpse const&) final;
		void visit(ItemBox const&) final;

		//! The animation produced by the last visit. Moves the animation out of the animator.
		uptr<Animation> animation() { return std::move(_animation); }
	private:
		uptr<Animation> _animation;
	};
}
