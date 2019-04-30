//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "ui/dialog.hpp"

#include "magic/spell.hpp"
#include "world/world.hpp"

namespace ql::qte {
	//! Quick time event for incanting a spell.
	struct incant : dialog {
		//! @param gatestone The gatestone used for incantation.
		//! @param cont The dialog continuation function.
		incant(sf::Window const& window,
			rsrc::fonts const& fonts,
			gatestone& gatestone,
			std::function<void(std::optional<magic::spell> const&)> cont);

		~incant();

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		enum class note { left, right, up, down };
		enum class side { left, right };

		static constexpr double time_factor = 4.0;

		gatestone& _gatestone;
		continuation<std::optional<magic::spell> const&> _cont;

		side _side = side::right;

		bool _begun = false;

		//! Amount of time since the dialog started.
		sec _total_elapsed_time = 0.0_s;

		std::vector<note> _notes;

		sf::Text _title;
		sf::Text _prompt;
		sf::RectangleShape _metronome;
	};
}
