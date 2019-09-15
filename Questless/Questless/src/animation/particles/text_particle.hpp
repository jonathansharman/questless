//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "particle.hpp"

#include <string>

namespace ql {
	//! A particle composed of colored text.
	struct text_particle : particle {
		//! @param lifetime The amount of time before this particle expires.
		//! @param font The font to use to render this text particle's text. Must out-live this particle.
		//! @param text The text for the particle to show.
		//! @param fill_color The fill color of the particle's text.
		//! @param outline_color The outline color of the particle's text.
		//! @param outline_thickness The thickness of the outline of the particle's text.
		text_particle( //
			sec lifetime,
			sf::Font const& font,
			sf::String const& text,
			sf::Color fill_color,
			sf::Color outline_color,
			float outline_thickness);

	private:
		sf::Text _text;
		sf::Color _base_fill_color;
		sf::Color _base_outline_color;

		auto particle_subupdate(sec) -> void final;

		auto particle_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
