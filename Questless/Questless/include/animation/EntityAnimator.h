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
#include "AnimationCollection.h"
#include "sdl-wrappers/resources.h"

namespace questless
{
	class EntityAnimator : public EntityVisitor
	{
	public:
		void visit(const Human&) override;
		void visit(const Goblin&) override;

		AnimationCollection::ptr animation() { return std::move(_animation); }
	private:
		AnimationCollection::ptr _animation;
	};
}

#endif
