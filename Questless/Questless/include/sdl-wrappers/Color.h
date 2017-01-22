/**
* @file    Color.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A color type, convertible to SDL_Color.
*/

#pragma once

#include <cstdint>

#include <SDL.h>

namespace sdl
{
	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		static constexpr Color clear() { return Color{0, 0, 0, 0}; }
		static constexpr Color white(uint8_t alpha = 255) { return Color{255, 255, 255, alpha}; }
		static constexpr Color silver(uint8_t alpha = 255) { return Color{192, 192, 192, alpha}; }
		static constexpr Color gray(uint8_t alpha = 255) { return Color{128, 128, 128, alpha}; }
		static constexpr Color black(uint8_t alpha = 255) { return Color{0, 0, 0, alpha}; }
		static constexpr Color red(uint8_t alpha = 255) { return Color{255, 0, 0, alpha}; }
		static constexpr Color lime(uint8_t alpha = 255) { return Color{0, 255, 0, alpha}; }
		static constexpr Color green(uint8_t alpha = 255) { return Color{0, 192, 0, alpha}; }
		static constexpr Color blue(uint8_t alpha = 255) { return Color{0, 0, 255, alpha}; }
		static constexpr Color cyan(uint8_t alpha = 255) { return Color{0, 255, 255, alpha}; }
		static constexpr Color yellow(uint8_t alpha = 255) { return Color{255, 255, 0, alpha}; }
		static constexpr Color magenta(uint8_t alpha = 255) { return Color{255, 0, 255, alpha}; }
		static constexpr Color orange(uint8_t alpha = 255) { return Color{255, 128, 0, alpha}; }
		static constexpr Color purple(uint8_t alpha = 255) { return Color{128, 0, 128, alpha}; }
		static constexpr Color teal(uint8_t alpha = 255) { return Color{0, 128, 128, alpha}; }
		static constexpr Color tan(uint8_t alpha = 255) { return Color{192, 160, 128, alpha}; }
		static constexpr Color brown(uint8_t alpha = 255) { return Color{128, 64, 0, alpha}; }

		constexpr explicit Color() : r{0}, g{0}, b{0}, a{255} {}
		constexpr explicit Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r{r}, g{g}, b{b}, a{a} {}
		constexpr explicit Color(Color color, uint8_t a) : r{color.r}, g{color.g}, b{color.b}, a{a} {}

		constexpr friend bool operator ==(Color left, Color right)
		{
			return left.r == right.r && left.g == right.g && left.b == right.b && left.a == right.a;
		}
		constexpr friend bool operator !=(Color left, Color right)
		{
			return left.r != right.r || left.g != right.g || left.b != right.b || left.a != right.a;
		}

		Color operator *(double factor) const
		{
			return Color
				{ static_cast<uint8_t>(factor * this->r)
				, static_cast<uint8_t>(factor * this->g)
				, static_cast<uint8_t>(factor * this->b)
				, this->a
				};
		}

		Color& operator *=(double factor)
		{
			r = static_cast<uint8_t>(factor * r);
			g = static_cast<uint8_t>(factor * g);
			b = static_cast<uint8_t>(factor * b);
			return *this;
		}

		constexpr operator SDL_Color() const { return {r, g, b, a}; }
	};
}
