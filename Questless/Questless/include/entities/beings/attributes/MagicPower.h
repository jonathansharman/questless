/**
* @file    MagicPower.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The MagicPower class, which encapsulates a being's proficiency with the six forms of magic.
*/

#ifndef MAGIC_POWER_H
#define MAGIC_POWER_H

#include "spell/Spell.h"

/// @todo Combine this file with MagicResistance using a common template Magic<T>, with T = Resistance or Power (will need to choose a different name for resistance probably).

namespace questless
{
	struct MagicPower
	{
		double white;
		double black;
		double green;
		double red;
		double blue;
		double yellow;

		/// @return The base magic power of the given Being subclass.
		template <typename BeingType>
		static MagicPower of()
		{
			using T = BeingType;
			return MagicPower
				{ T::base_white_power
				, T::base_black_power
				, T::base_green_power
				, T::base_red_power
				, T::base_blue_power
				, T::base_yellow_power
				};
		}

		MagicPower() : white{0}, black{0}, green{0}, red{0}, blue{0}, yellow{0} {}

		MagicPower(double white, double black, double green, double red, double blue, double yellow)
			: white{white}, black{black}, green{green}, red{red}, blue{blue}, yellow{yellow}
		{}

		friend std::ostream& operator <<(std::ostream& out, MagicPower mp)
		{
			out << mp.white << ' ' << mp.black << ' ' << mp.green << ' ' << mp.red << ' ' << mp.blue << ' ' << mp.yellow;
			return out;
		}

		friend std::istream& operator >> (std::istream& in, MagicPower mp)
		{
			in >> mp.white >> mp.black >> mp.green >> mp.red >> mp.blue >> mp.yellow;
			return in;
		}

		template <spell::Color color> double get() const;
		template <> double get<spell::Color::white>() const { return white; }
		template <> double get<spell::Color::black>() const { return black; }
		template <> double get<spell::Color::green>() const { return green; }
		template <> double get<spell::Color::red>() const { return red; }
		template <> double get<spell::Color::blue>() const { return blue; }
		template <> double get<spell::Color::yellow>() const { return yellow; }

		template <spell::Color color> double& get();
		template <> double& get<spell::Color::white>() { return white; }
		template <> double& get<spell::Color::black>() { return black; }
		template <> double& get<spell::Color::green>() { return green; }
		template <> double& get<spell::Color::red>() { return red; }
		template <> double& get<spell::Color::blue>() { return blue; }
		template <> double& get<spell::Color::yellow>() { return yellow; }

		/// Sets all magic power levels that are below their minimum values to their minimum values.
		void clamp()
		{
			if (white < 0.0) { white = 0.0; }
			if (black < 0.0) { black = 0.0; }
			if (green < 0.0) { green = 0.0; }
			if (red < 0.0) { red = 0.0; }
			if (blue < 0.0) { blue = 0.0; }
			if (yellow < 0.0) { yellow = 0.0; }
		}
	};
}

#endif
