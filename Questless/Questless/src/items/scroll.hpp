//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

#include "item.hpp"
#include "magic/spell.hpp"

namespace ql {
	//! A magic scroll for inscribing spells.
	class scroll : public item_base<scroll> {
	public:
		scroll(uptr<magic::spell> spell, ql::id<item> id = ql::id<item>::make()) : item_base{id}, _spell{std::move(spell)} {}

		std::string name() const final { return blank() ? "Blank Scroll" : "Scroll: " + _spell->name(); }

		load mass() const final { return 0.1_load; }

		bool blank() const { return _spell == nullptr; }

		magic::spell& spell() { return *_spell; }
		magic::spell const& spell() const { return *_spell; }

		std::vector<uptr<action>> actions() final;
	private:
		uptr<magic::spell> _spell;
	};
}
