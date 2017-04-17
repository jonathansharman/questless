//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "spell/Spell.h"
#include "utility/TaggedType.h"
#include "utility/Bounded.h"

namespace questless
{
	struct WhiteMagic : TaggedType<double> { using TaggedType::TaggedType; };
	struct BlackMagic : TaggedType<double> { using TaggedType::TaggedType; };
	struct GreenMagic : TaggedType<double> { using TaggedType::TaggedType; };
	struct RedMagic : TaggedType<double> { using TaggedType::TaggedType; };
	struct BlueMagic : TaggedType<double> { using TaggedType::TaggedType; };
	struct YellowMagic : TaggedType<double> { using TaggedType::TaggedType; };

	//! Base type for Magic and Antimagic, using CRTP.
	template <typename Derived>
	struct MagicStat
	{
		static constexpr double minimum_value = 0.0;

		Bounded<double, minimum_value> white = 0.0;
		Bounded<double, minimum_value> black = 0.0;
		Bounded<double, minimum_value> green = 0.0;
		Bounded<double, minimum_value> red = 0.0;
		Bounded<double, minimum_value> blue = 0.0;
		Bounded<double, minimum_value> yellow = 0.0;

		constexpr MagicStat() = default;

		constexpr MagicStat(WhiteMagic white, BlackMagic black, GreenMagic green, RedMagic red, BlueMagic blue, YellowMagic yellow)
			: white{std::max(0.0, white.value)}, black{std::max(0.0, black.value)}, green{std::max(0.0, green.value)}, red{std::max(0.0, red.value)}, blue{std::max(0.0, blue.value)}, yellow{std::max(0.0, yellow.value)}
		{}

		static constexpr Derived zero() { return Derived{}; }

		friend std::ostream& operator <<(std::ostream& out, MagicStat ma)
		{
			out << ma.white << ' ' << ma.black << ' ' << ma.green << ' ' << ma.red << ' ' << ma.blue << ' ' << ma.yellow << ' ';
			return out;
		}

		friend std::istream& operator >>(std::istream& in, MagicStat ma)
		{
			in >> ma.white >> ma.black >> ma.green >> ma.red >> ma.blue >> ma.yellow;
			return in;
		}

		template <spell::Color color> constexpr double get() const;
		template <> constexpr double get<spell::Color::white>() const { return _white(); }
		template <> constexpr double get<spell::Color::black>() const { return _black(); }
		template <> constexpr double get<spell::Color::green>() const { return _green(); }
		template <> constexpr double get<spell::Color::red>() const { return _red(); }
		template <> constexpr double get<spell::Color::blue>() const { return _blue(); }
		template <> constexpr double get<spell::Color::yellow>() const { return _yellow(); }

		template <spell::Color color> void set(double value)
		{
			if constexpr(color == spell::Color::white)  white(value);
			if constexpr(color == spell::Color::black)  black(value);
			if constexpr(color == spell::Color::green)  green(value);
			if constexpr(color == spell::Color::red)    red(value);
			if constexpr(color == spell::Color::blue)   blue(value);
			if constexpr(color == spell::Color::yellow) yellow(value);
		}
	};

	//! A being's proficiency with the six forms of magic.
	class Magic : public MagicStat<Magic>
	{
	public:
		using MagicStat<Magic>::MagicStat;
	};

	//! A being's resistance to the six forms of magic.
	class Antimagic : public MagicStat<Antimagic>
	{
	public:
		using MagicStat<Antimagic>::MagicStat;
	};
}
