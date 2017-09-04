//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/SpellTexturer.h"
#include "sdl/resources.h"
#include "items/Scroll.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void SpellTexturer::visit(spell::EagleEye const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/spells/eagle-eye.png");
		_texture_handle = texture_handle;
	}

	void SpellTexturer::visit(spell::Heal const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/spells/heal.png");
		_texture_handle = texture_handle;
	}

	void SpellTexturer::visit(spell::Shock const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/spells/shock.png");
		_texture_handle = texture_handle;
	}

	void SpellTexturer::visit(spell::Teleport const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/spells/teleport.png");
		_texture_handle = texture_handle;
	}
}
