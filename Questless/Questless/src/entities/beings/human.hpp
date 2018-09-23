//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/being.hpp"

namespace ql {
	class human : public being_base<human> {
	public:
		human(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id = ql::id<being>::make());

		ql::entity_subtype entity_subtype() const final { return entity_subtype::human_class; }

		double transparency() const final { return 0.5; }

		std::string const& description() const { //! @todo Put this in being or entity?
			return "You already know about humans.";
		}
	protected:
		ql::body make_body(ql::id<being> owner_id) final;
	};
}
