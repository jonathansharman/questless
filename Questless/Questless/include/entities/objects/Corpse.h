/**
* @file    Corpse.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The remains of a deceased corporeal being.
*/

#pragma once

#include "Object.h"
#include "entities/beings/Being.h"

namespace questless
{
	class Corpse : public Object
	{
	public:
		Corpse(Game& game, ObjectId id, Being::ptr being)
			: Object(game, id)
			, _being{std::move(being)}
		{}
		Corpse(Game& game, std::istream& in, Being::ptr being)
			: Object(game, in)
			, _being{std::move(being)}
		{}

		void accept(EntityVisitor& visitor) override { visitor.visit(*this); }
		void accept(EntityVisitor& visitor) const override { visitor.visit(*this); }

		virtual EntityClass entity_class() const { return EntityClass::CorpseClass; }

		Being const& being() const { return *_being; }
		Being& being() { return *_being; }
	private:
		Being::ptr _being;
	};
}
