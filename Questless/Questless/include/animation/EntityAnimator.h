/**
* @file    EntityAnimator.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the entity animator class, which generates animations for entities.
*/

#ifndef ENTITY_ANIMATOR_H
#define ENTITY_ANIMATOR_H

#include "entities/EntityVisitor.h"
#include "AnimationSet.h"
#include "sdl-wrappers/resources.h"

namespace questless
{
	class EntityAnimator : public EntityVisitor
	{
	public:
		void visit(const Human&) override;
		void visit(const Goblin&) override;

		void visit(const Corpse&) override;
		void visit(const ItemBox&) override;

		/// @return The animation set produced by the last visit. Moves the animation set out of the animator.
		AnimationSet::ptr animation_set() { return std::move(_animation_set); }
	private:
		AnimationSet::ptr _animation_set;
	};
}

#endif
