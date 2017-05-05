//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "AnimationSet.h"
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

		void visit(Corpse const&) final;
		void visit(ItemBox const&) final;

		//! The animation set produced by the last visit.
		//! @note Throws std::bad_optional_access if this visitor hasn't visited an entity.
		AnimationSet& animation_set() { return *_animation_set; }
	private:
		std::optional<AnimationSet> _animation_set;
	};
}
