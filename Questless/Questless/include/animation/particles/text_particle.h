//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/particle.h"
#include "sdl/renderable.h"
#include "utility/utility.h"
#include "utility/initializer.h"

namespace ql
{
	//! A particle composed of colored text.
	class text_particle : public particle, public sdl::renderable
	{
	public:
		//! @param position The particle's starting position.
		//! @param text The text for the particle to show.
		//! @param color The color of the particle's text.
		text_particle(std::string text, units::colors::color color)
			: particle
				{ units::game_space::vector{0.0, 0.0}
				, (random_displacement(120.0) + units::game_space::vector{0.0, 160.0}) / 1.0s
				, units::game_space::vector{0.0, -400.0} / 1.0s / 1.0s
				, units::game_space::radians{0.0}
				, units::game_space::radians{0.0} / 1.0s
				, scale{1.0}
				, units::game_space::scale_velocity{0.0}
				, lifetime{2.0s}
				}
			, _text{std::move(text)}
			, _color{color}
			, _texture{make_texture()}
		{}
	private:
		static sdl::font_handle _font_handle;

		friend class initializer<text_particle>;
		static initializer<text_particle> _initializer;
		static void initialize();

		std::string _text;
		units::colors::color _color;

		sdl::texture _texture;

		void particle_subupdate() final {}

		sdl::texture const& texture() const final { return _texture; }

		void refresh() final { _texture = make_texture(); }

		sdl::texture make_texture() const;
	};
}
