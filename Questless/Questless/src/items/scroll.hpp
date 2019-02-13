//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "item.hpp"
#include "magic/spell.hpp"

#include <memory>
#include <optional>

namespace ql {
	//! A magic scroll for inscribing spells.
	struct scroll : item_base<scroll> {
		scroll(std::optional<magic::spell> spell, ql::id<item> id = ql::id<item>::make())
			: item_base{id}, _spell{std::move(spell)} {}

		std::string name() const final {
			return blank() ? "Blank Scroll" : "Scroll: " + _spell->name();
		}

		ql::mass mass() const final {
			return 0.1_mass;
		}

		bool blank() const {
			return _spell == std::nullopt;
		}

		magic::spell& spell() {
			return *_spell;
		}
		magic::spell const& spell() const {
			return *_spell;
		}

		std::vector<uptr<action>> actions() final;

	private:
		std::optional<magic::spell> _spell;
	};
}
