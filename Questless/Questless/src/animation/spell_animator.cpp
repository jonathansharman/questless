//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "spell_animator.hpp"

#include "still_image.hpp"

#include "items/scroll.hpp"
#include "rsrc/spell.hpp"

using namespace media;

namespace ql {
	spell_animator::~spell_animator() = default;

	void spell_animator::visit(magic::telescope const&) {
		animation = umake<still_image>(resources.telescope);
	}

	void spell_animator::visit(magic::heal const&) {
		animation = umake<still_image>(resources.heal);
	}

	void spell_animator::visit(magic::shock const&) {
		animation = umake<still_image>(resources.shock);
	}

	void spell_animator::visit(magic::teleport const&) {
		animation = umake<still_image>(resources.teleport);
	}
}
