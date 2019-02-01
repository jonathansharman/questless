//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>

#include "animation/particles/particle.hpp"

namespace ql {
	//! A particle composed of colored text.
	class text_particle : public particle {
	public:
		//! @param displacement The initial displacement of this particle.
		//! @param lifetime The amount of time before this particle expires.
		//! @param font The font to use to render this text particle's text. Must out-live this particle.
		//! @param text The text for the particle to show.
		//! @param color The color of the particle's text.
		text_particle(world::vector displacement, sec lifetime, sf::Font const& font, sf::String const& text, sf::Color color);

	private:
		sf::Text _text;

		void particle_subupdate(sec) final {}

		void particle_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;
	};
}
