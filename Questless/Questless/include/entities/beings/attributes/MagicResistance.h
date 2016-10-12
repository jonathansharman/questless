/**
* @file    MagicResistance.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The MagicPower class, which encapsulates a being's proficiency with the six forms of magic.
*/

#ifndef MAGIC_RESISTANCE_H
#define MAGIC_RESISTANCE_H

namespace questless
{
	struct MagicResistance
	{
		double white;
		double black;
		double green;
		double red;
		double blue;
		double yellow;

		/// @return The base magic resistance of the given Being subclass.
		template <typename BeingType>
		static MagicResistance of()
		{
			using T = BeingType;
			return MagicResistance
				{ T::base_white_power
				, T::base_black_power
				, T::base_green_power
				, T::base_red_power
				, T::base_blue_power
				, T::base_yellow_power
				};
		}

		MagicResistance() : white{0}, black{0}, green{0}, red{0}, blue{0}, yellow{0} {}

		MagicResistance(double white, double black, double green, double red, double blue, double yellow)
			: white{white}, black{black}, green{green}, red{red}, blue{blue}, yellow{yellow}
		{}

		friend std::ostream& operator <<(std::ostream& out, MagicResistance mr)
		{
			out << mr.white << ' ' << mr.black << ' ' << mr.green << ' ' << mr.red << ' ' << mr.blue << ' ' << mr.yellow;
			return out;
		}

		friend std::istream& operator >> (std::istream& in, MagicResistance mr)
		{
			in >> mr.white >> mr.black >> mr.green >> mr.red >> mr.blue >> mr.yellow;
			return in;
		}

		template <Spell::Color color> double& get();
		template <> double& get<Spell::Color::white>() { return white; }
		template <> double& get<Spell::Color::black>() { return black; }
		template <> double& get<Spell::Color::green>() { return green; }
		template <> double& get<Spell::Color::red>() { return red; }
		template <> double& get<Spell::Color::blue>() { return blue; }
		template <> double& get<Spell::Color::yellow>() { return yellow; }

		/// Sets all magic resistance levels that are below their minimum values to their minimum values.
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
