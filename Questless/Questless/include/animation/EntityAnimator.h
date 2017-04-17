//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/EntityVisitor.h"
#include "AnimationSet.h"
#include "sdl/resources.h"

namespace questless
{
	//! Creates animations for entities.
	class EntityAnimator : public EntityVisitor
	{
	public:
		void visit(Human const&) override;
		void visit(Goblin const&) override;

		void visit(Corpse const&) override;
		void visit(ItemBox const&) override;

		//! The animation set produced by the last visit. Moves the animation set out of the animator.
		AnimationSet::uptr animation_set() { return std::move(_animation_set); }
	private:
		AnimationSet::uptr _animation_set;
	};
}
