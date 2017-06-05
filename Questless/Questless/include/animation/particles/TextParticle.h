//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/Particle.h"
#include "sdl/Renderable.h"
#include "utility/utility.h"
#include "utility/Initializer.h"

namespace questless
{
	//! A particle composed of colored text.
	class TextParticle : public Particle, public sdl::Renderable
	{
	public:
		//! @param position The particle's starting position.
		//! @param text The text for the particle to show.
		//! @param color The color of the particle's text.
		TextParticle(std::string text, units::colors::Color color)
			: Particle
				{ units::GameSpace::Vector{0.0, 0.0}
				, (random_displacement(120.0) + units::GameSpace::Vector{0.0, 160.0}) / 1.0s
				, units::GameSpace::Vector{0.0, -400.0} / 1.0s / 1.0s
				, units::GameSpace::Radians{0.0}
				, units::GameSpace::Radians{0.0} / 1.0s
				, Scale{1.0}
				, units::GameScaleVelocity{0.0}
				, Lifetime{2.0s}
				}
			, _text{std::move(text)}
			, _color{color}
			, _texture{make_texture()}
		{}
	private:
		static sdl::FontHandle _font_handle;

		friend class Initializer<TextParticle>;
		static Initializer<TextParticle> _initializer;
		static void initialize();

		std::string _text;
		units::colors::Color _color;

		sdl::Texture _texture;

		void particle_subupdate() final {}

		sdl::Texture const& texture() const final { return _texture; }

		void refresh() final { _texture = make_texture(); }

		sdl::Texture make_texture() const;
	};
}
