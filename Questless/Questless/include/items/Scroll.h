//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

#include "Item.h"
#include "spell/Spell.h"

namespace questless
{
	//! A magic scroll for inscribing spells.
	class Scroll : public ItemBase<Scroll>
	{
	public:
		Scroll(uptr<spell::Spell> spell, Id<Item> id = Id<Item>::make()) : ItemBase{id}, _spell{std::move(spell)} {}

		std::string name() const final { return blank() ? "Blank Scroll" : "Scroll: " + _spell->name(); }

		double weight() const final { return 0.1; }

		bool blank() const { return _spell == nullptr; }

		spell::Spell& spell() { return *_spell; }
		spell::Spell const& spell() const { return *_spell; }

		std::vector<uptr<Action>> actions() final;

		void update() final { _spell->update(); }
	private:
		uptr<spell::Spell> _spell;
	};
}
