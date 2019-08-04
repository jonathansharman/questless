//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "ui/dialog/dialog.hpp"

#include "magic/spell.hpp"
#include "world/world_space.hpp"

namespace ql {
	struct gatestone;

	namespace qte {
		//! Quick time event for incanting a spell.
		struct incant : dialog<std::optional<magic::spell>> {
			//! @param gatestone The gatestone used for incantation.
			//! @param cont The dialog continuation function.
			incant(sf::Window const& window, rsrc::fonts const& fonts, gatestone& gatestone);

			void update(sec elapsed_time, std::vector<sf::Event>& events) final;

		private:
			enum class note { left, right, up, down };
			enum class side { left, right };

			static constexpr double time_factor = 4.0;

			gatestone& _gatestone;

			side _side = side::right;

			bool _begun = false;

			//! Amount of time since the dialog started.
			sec _total_elapsed_time = 0.0_s;

			std::vector<note> _notes;

			label _title;
			label _prompt;
			sf::RectangleShape _metronome;

			void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
		};
	}
}
