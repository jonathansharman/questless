//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "text_particle.hpp"

#include "quantities/wall_time.hpp"
#include "utility/random.hpp"

using namespace ql::view::literals;

namespace ql {
	text_particle::text_particle( //
		sec lifetime,
		sf::Font const& font,
		sf::String const& text,
		sf::Color fill_color,
		sf::Color outline_color,
		float outline_thickness)
		: particle{lifetime}
		, _base_fill_color{fill_color}
		, _base_outline_color{outline_color} //
	{
		_text.setFont(font);
		_text.setString(text);
		_text.setOutlineThickness(outline_thickness);

		// Set origin to the center of the text by default.
		setOrigin({_text.getLocalBounds().width / 2, _text.getLocalBounds().height / 2});

		velocity = (random_displacement(120.0_px) + view::vector{0.0_px, 160.0_px}) / 1.0_s;
		acceleration = view::vector{0.0_px, -400.0_px} / 1.0_s / 1.0_s;
	}

	auto text_particle::particle_subupdate(sec) -> void {
		_text.setFillColor(_base_fill_color * color_factor);
		_text.setOutlineColor(_base_outline_color * color_factor);
	}

	auto text_particle::particle_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		target.draw(_text, states);
	}
}
