/**
* @file    EntityAnimator.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the entity animator class, which generates animations for entities.
*/

#pragma once

#include "entities/EntityVisitor.h"
#include "AnimationSet.h"
#include "sdl/resources.h"

namespace questless
{
	class EntityAnimator : public EntityVisitor
	{
	public:
		void visit(Human const&) override;
		void visit(Goblin const&) override;

		void visit(Corpse const&) override;
		void visit(ItemBox const&) override;

		/// @return The animation set produced by the last visit. Moves the animation set out of the animator.
		AnimationSet::uptr animation_set() { return std::move(_animation_set); }
	private:
		AnimationSet::uptr _animation_set;
	};
}
