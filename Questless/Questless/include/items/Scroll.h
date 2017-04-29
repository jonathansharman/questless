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
	class Scroll : public ItemConstBase<Scroll>
	{
	public:
		Scroll(uptr<spell::Spell> spell, Id<Item> id = Id<Item>::make()) : ItemConstBase{id}, _spell{std::move(spell)} {}

		std::string name() const override { return blank() ? "Blank Scroll" : "Scroll: " + _spell->name(); }

		double weight() const override { return 0.1; }

		bool blank() const { return _spell == nullptr; }

		spell::Spell& spell() { return *_spell; }
		spell::Spell const& spell() const { return *_spell; }

		std::vector<uptr<Action>> actions() override;

		void update() override { _spell->update(); }
	private:
		uptr<spell::Spell> _spell;
	};
}
