//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Object.h"
#include "entities/beings/Being.h"

namespace questless
{
	//! The remains of a deceased corporeal being.
	class Corpse : public ObjectBase<Corpse>
	{
	public:
		Corpse(Id<Being> being_id, Id<Object> id = Id<Object>::make())
			: ObjectBase<Corpse>{id}
			, _being_id{being_id}
		{}
		Corpse(std::istream& in)
			: ObjectBase<Corpse>{in}
			, _being_id{in}
		{}

		virtual EntityClass entity_class() const { return EntityClass::CorpseClass; }

		double transparency() const final { return 0.5; }

		bool blocks_movement() const final { return true; }

		Id<Being> being_id() const { return _being_id; }
	private:
		Id<Being> const _being_id;
	};
}
