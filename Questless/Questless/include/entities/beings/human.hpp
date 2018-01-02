//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/corporeal_being.hpp"

namespace ql
{
	class human : public corporeal_being_base<human>
	{
	public:
		human(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id = ql::id<being>::make());
		human(std::istream& in);

		ql::entity_class entity_class() const final { return entity_class::human_class; }

		void serialize(std::ostream& out) const final;

		double transparency() const final { return 0.5; }

		std::string const& description() const //! @todo Put this in being or entity?
		{
			return "You already know about humans.";
		}
	protected:
		ql::body make_body(ql::id<being> owner_id) final;
	};
}
