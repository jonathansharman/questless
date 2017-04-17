//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Object.h"
#include "entities/beings/Being.h"

namespace questless
{
	//! The remains of a deceased corporeal being.
	class Corpse : public Object
	{
	public:
		Corpse(Id<Being> being_id, Id<Object> id = Id<Object>::make())
			: Object{id}
			, _being_id{being_id}
		{}
		Corpse(std::istream& in)
			: Object{in}
			, _being_id{in}
		{}

		void accept(EntityVisitor& visitor) override { visitor.visit(*this); }
		void accept(EntityVisitor& visitor) const override { visitor.visit(*this); }

		virtual EntityClass entity_class() const { return EntityClass::CorpseClass; }

		bool blocks_movement() const override { return true; }

		Id<Being> being_id() const { return _being_id; }
	private:
		Id<Being> const _being_id;
	};
}
