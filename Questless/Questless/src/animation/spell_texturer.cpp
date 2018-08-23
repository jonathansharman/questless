//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/spell_texturer.hpp"
#include "sdl/resources.hpp"
#include "items/scroll.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	void spell_texturer::visit(magic::eagle_eye const&) {
		static auto texture_handle = the_texture_manager().add("resources/textures/spells/eagle-eye.png");
		_texture_handle = texture_handle;
	}

	void spell_texturer::visit(magic::heal const&) {
		static auto texture_handle = the_texture_manager().add("resources/textures/spells/heal.png");
		_texture_handle = texture_handle;
	}

	void spell_texturer::visit(magic::shock const&) {
		static auto texture_handle = the_texture_manager().add("resources/textures/spells/shock.png");
		_texture_handle = texture_handle;
	}

	void spell_texturer::visit(magic::teleport const&) {
		static auto texture_handle = the_texture_manager().add("resources/textures/spells/teleport.png");
		_texture_handle = texture_handle;
	}
}
