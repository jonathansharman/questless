/**
* @file    TextParticle.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A text particle.
*/

#ifndef TEXT_PARTICLE_H
#define TEXT_PARTICLE_H

#include "animation/particles/Particle.h"
#include "sdl-wrappers/Renderable.h"
#include "utility/utility.h"
#include "utility/Initializer.h"

namespace questless
{
	class TextParticle : public Particle, public sdl::Renderable
	{
	public:
		/// @param position The particle's starting position.
		/// @param text The text for the particle to show.
		/// @param color The color of the particle's text.
		TextParticle(units::GamePoint position, std::string text, sdl::Color color)
			: Particle
				{ position
				, (random_displacement(120.0) + units::GameVector{0.0, 160.0}) / 1.0s
				, units::GameVector{0.0, -400.0} / 1.0s / 1.0s
				, units::GameRadians{0.0}
				, units::GameRadians{0.0} / 1.0s
				, Scale{1.0}
				, units::GameScaleVelocity{0.0}
				, Lifetime{2.0s}
				}
			, _text{std::move(text)}
			, _color{color}
			, _texture{make_texture()}
		{}

		/// @param position The particle's starting position.
		static ptr make(units::GamePoint position, std::string text, sdl::Color color) { return std::make_unique<TextParticle>(position, std::move(text), color); }
	private:
		friend class Initializer<TextParticle>;
		static Initializer<TextParticle> _initializer;
		static void initialize();

		static sdl::Handle<sdl::Font> _font_handle;

		std::string _text;
		sdl::Color _color;

		sdl::Texture _texture;

		void subupdate() override {}

		const sdl::Texture& texture() const override { return _texture; }

		void refresh() override { _texture = make_texture(); }

		sdl::Texture make_texture() const;
	};
}

#endif
