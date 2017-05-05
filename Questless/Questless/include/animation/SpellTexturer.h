//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "spell/SpellVisitor.h"
#include "sdl/resources.h"
#include "utility/reference.h"

namespace questless
{
	//! Creates textures for items.
	class SpellTexturer : public spell::SpellConstVisitor
	{
	public:
		void visit(spell::EagleEye const&) final;
		void visit(spell::Heal const&) final;
		void visit(spell::LightningBolt const&) final;
		void visit(spell::Teleport const&) final;

		//! A handle to the texture of the last visited item.
		auto texture_handle() const { return _texture_handle; }
	private:
		sdl::Handle<sdl::Texture> _texture_handle;
	};
}
