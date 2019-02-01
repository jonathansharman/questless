//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation.hpp"

#include "magic/spell_visitor.hpp"
#include "utility/reference.hpp"

namespace ql {
	namespace rsrc {
		struct spell;
	}
	class animation;

	//! Creates animations for spells.
	struct spell_animator : public magic::spell_const_visitor {
		rsrc::spell const& resources;

		//! The animation created by the last visit.
		uptr<animation> animation = nullptr;

		spell_animator(rsrc::spell const& resources) : resources{resources} {}

		~spell_animator();

		void visit(magic::telescope const&) final;
		void visit(magic::heal const&) final;
		void visit(magic::shock const&) final;
		void visit(magic::teleport const&) final;
	};
}
