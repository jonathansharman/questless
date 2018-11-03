//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "text_particle.hpp"

#include "sdl/resources.hpp"
#include "utility/random.hpp"

using namespace units;

namespace ql {
	//! @param position The particle's starting position.
	//! @param text The text for the particle to show.
	//! @param color The color of the particle's text.
	text_particle::text_particle(std::string text, colors::color color)
		: particle
			{ world_space::vector::zero()
			, (random_displacement(120.0) + world_space::vector{0.0, 160.0}) / 1.0s
			, world_space::vector{0.0, -400.0} / 1.0s / 1.0s
			, world_space::radians{0.0}
			, world_space::radians{0.0} / 1.0s
			, 1.0
			, world_space::scale_velocity{0.0}
			, 2.0s
			}
		, _text{std::move(text)}
		, _color{color}
		, _texture{make_texture()}
	{}

	sdl::texture text_particle::make_texture() const {
		static auto font_handle = sdl::the_font_manager().add("resources/fonts/firamono.ttf", 20);
		return sdl::the_font_manager()[font_handle].render(_text.c_str(), _color);
	}
}
