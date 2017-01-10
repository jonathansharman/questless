/**
* @file    Scroll.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Scroll class.
*/

#ifndef SCROLL_H
#define SCROLL_H

#include <memory>

#include "Item.h"
#include "spell/Spell.h"

namespace questless
{
	class Scroll : public Item
	{
	public:
		Scroll(spell::Spell::ptr spell) : _spell{std::move(spell)} {}

		void accept(ItemVisitor& visitor) const override { return visitor.visit(*this); }

		std::string name() const override { return blank() ? "Blank Scroll" : "Scroll"; }

		double weight() const override { return 0.1; }

		bool blank() const { return _spell == nullptr; }

		spell::Spell& spell() { return *_spell; }
		const spell::Spell& spell() const { return *_spell; }

		std::vector<Action::ptr> actions() override;

		void update() override { _spell->update(); }
	private:
		spell::Spell::ptr _spell;
	};
}

#endif
