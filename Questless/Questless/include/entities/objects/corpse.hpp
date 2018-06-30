//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "object.hpp"
#include "entities/beings/being.hpp"

namespace ql
{
	//! The remains of a deceased corporeal being.
	class corpse : public object_base<corpse>
	{
	public:
		corpse(ql::id<being> being_id, ql::id<object> id = ql::id<object>::make())
			: object_base<corpse>{id}
			, _being_id{being_id}
		{}
		corpse(std::istream& in)
			: object_base<corpse>{in}
			, _being_id{in}
		{}

		virtual ql::entity_subtype entity_subtype() const { return entity_subtype::corpse_class; }

		double transparency() const final { return 0.5; }

		bool blocks_movement() const final { return true; }

		ql::id<being> being_id() const { return _being_id; }
	private:
		ql::id<being> const _being_id;
	};
}
