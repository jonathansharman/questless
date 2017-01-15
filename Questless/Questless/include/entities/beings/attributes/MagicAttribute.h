/**
* @file    MagicPower.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines MagicPower and MagicResistance, which represent a being's proficiency with or resistance to the six forms of magic, respectively.
*/

#ifndef MAGIC_ATTRIBUTE_H
#define MAGIC_ATTRIBUTE_H

#include "spell/Spell.h"
#include "utility/TaggedType.h"

namespace questless
{
	struct WhiteMagic : TaggedType<double> { using TaggedType::TaggedType; };
	struct BlackMagic : TaggedType<double> { using TaggedType::TaggedType; };
	struct GreenMagic : TaggedType<double> { using TaggedType::TaggedType; };
	struct RedMagic : TaggedType<double> { using TaggedType::TaggedType; };
	struct BlueMagic : TaggedType<double> { using TaggedType::TaggedType; };
	struct YellowMagic : TaggedType<double> { using TaggedType::TaggedType; };

	template <typename Derived>
	class MagicAttribute
	{
	public:
		constexpr MagicAttribute() = default;

		constexpr MagicAttribute(WhiteMagic white, BlackMagic black, GreenMagic green, RedMagic red, BlueMagic blue, YellowMagic yellow)
			: _white{std::max(0.0, white.value)}, _black{std::max(0.0, black.value)}, _green{std::max(0.0, green.value)}, _red{std::max(0.0, red.value)}, _blue{std::max(0.0, blue.value)}, _yellow{std::max(0.0, yellow.value)}
		{}

		constexpr static Derived zero() { return Derived{}; }

		friend std::ostream& operator <<(std::ostream& out, MagicAttribute ma)
		{
			out << ma._white << ' ' << ma._black << ' ' << ma._green << ' ' << ma._red << ' ' << ma._blue << ' ' << ma._yellow << ' ';
			return out;
		}

		friend std::istream& operator >> (std::istream& in, MagicAttribute ma)
		{
			double white, black, green, red, blue, yellow;
			in >> white >> black >> green >> red >> blue >> yellow;
			
			ma._white = std::max(0.0, white);
			ma._black = std::max(0.0, black);
			ma._green = std::max(0.0, green);
			ma._red = std::max(0.0, red);
			ma._blue = std::max(0.0, blue);
			ma._yellow = std::max(0.0, yellow);

			return in;
		}

		template <spell::Color color> constexpr double get() const;
		template <> constexpr double get<spell::Color::white>() const { return _white; }
		template <> constexpr double get<spell::Color::black>() const { return _black; }
		template <> constexpr double get<spell::Color::green>() const { return _green; }
		template <> constexpr double get<spell::Color::red>() const { return _red; }
		template <> constexpr double get<spell::Color::blue>() const { return _blue; }
		template <> constexpr double get<spell::Color::yellow>() const { return _yellow; }

		template <spell::Color color> void set(double value)
		{
			if constexpr(color == spell::Color::white)  white(value);
			if constexpr(color == spell::Color::black)  black(value);
			if constexpr(color == spell::Color::green)  green(value);
			if constexpr(color == spell::Color::red)    red(value);
			if constexpr(color == spell::Color::blue)   blue(value);
			if constexpr(color == spell::Color::yellow) yellow(value);
		}

		constexpr double white() const { return _white; }
		void white(double value) { _white = std::max(0.0, value); }

		constexpr double black() const { return _black; }
		void black(double value) { _black = std::max(0.0, value); }

		constexpr double green() const { return _green; }
		void green(double value) { _green = std::max(0.0, value); }

		constexpr double red() const { return _red; }
		void red(double value) { _red = std::max(0.0, value); }

		constexpr double blue() const { return _blue; }
		void blue(double value) { _blue = std::max(0.0, value); }

		constexpr double yellow() const { return _yellow; }
		void yellow(double value) { _yellow = std::max(0.0, value); }
	private:
		double _white = 0.0;
		double _black = 0.0;
		double _green = 0.0;
		double _red = 0.0;
		double _blue = 0.0;
		double _yellow = 0.0;
	};

	class MagicPower : public MagicAttribute<MagicPower>
	{
	public:
		using MagicAttribute<MagicPower>::MagicAttribute;
	};

	class MagicResistance : public MagicAttribute<MagicResistance>
	{
	public:
		using MagicAttribute<MagicResistance>::MagicAttribute;
	};
}

#endif
