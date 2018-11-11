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
		//! @param position The particle's starting position.
		//! @param text The text for the particle to show.
		//! @param color The color of the particle's text.
		text_particle(std::string text, spaces::colors::color color);
	private:
		std::string _text;
		spaces::colors::color _color;

		sdl::texture _texture;

		void particle_subupdate() final {}

		sdl::texture const& texture() const final { return _texture; }

		sdl::texture make_texture() const;
	};
}
