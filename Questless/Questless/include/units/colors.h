//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines some mathematical constants under the namespace units::constants.

#pragma once

#include "color_space.h"

namespace units::colors
{
	using color = units::color_space::point;
	using color_factor = units::color_space::vector;

	constexpr color clear(float red = 0.0, float green = 0.0, float blue = 0.0) { return color{red, green, blue, 0.0f}; }
	constexpr color white(float alpha = 1.0) { return color{1.0f, 1.0f, 1.0f, alpha}; }
	constexpr color silver(float alpha = 1.0) { return color{0.75f, 0.75f, 0.75f, alpha}; }
	constexpr color gray(float alpha = 1.0) { return color{0.5f, 0.5f, 0.5f, alpha}; }
	constexpr color black(float alpha = 1.0) { return color{0.0f, 0.0f, 0.0f, alpha}; }
	constexpr color red(float alpha = 1.0) { return color{1.0f, 0.0f, 0.0f, alpha}; }
	constexpr color lime(float alpha = 1.0) { return color{0.0f, 1.0f, 0.0f, alpha}; }
	constexpr color green(float alpha = 1.0) { return color{0.0f, 0.75f, 0.0f, alpha}; }
	constexpr color blue(float alpha = 1.0) { return color{0.0f, 0.0f, 1.0f, alpha}; }
	constexpr color cyan(float alpha = 1.0) { return color{0.0f, 1.0f, 1.0f, alpha}; }
	constexpr color yellow(float alpha = 1.0) { return color{1.0f, 1.0f, 0.0f, alpha}; }
	constexpr color magenta(float alpha = 1.0) { return color{1.0f, 0.0f, 1.0f, alpha}; }
	constexpr color orange(float alpha = 1.0) { return color{1.0f, 0.5f, 0.0f, alpha}; }
	constexpr color purple(float alpha = 1.0) { return color{0.5f, 0.0f, 0.5f, alpha}; }
	constexpr color teal(float alpha = 1.0) { return color{0.0f, 0.5f, 0.5f, alpha}; }
	constexpr color tan(float alpha = 1.0) { return color{0.75f, 0.625f, 0.5f, alpha}; }
	constexpr color brown(float alpha = 1.0) { return color{0.5f, 0.25f, 0.0f, alpha}; }

	constexpr color_factor clear_factor(float alpha) { return color_factor{1.0f, 1.0f, 1.0f, alpha}; }
	constexpr color_factor white_factor(float alpha = 1.0) { return color_factor{1.0f, 1.0f, 1.0f, alpha}; }
	constexpr color_factor silver_factor(float alpha = 1.0) { return color_factor{0.75f, 0.75f, 0.75f, alpha}; }
	constexpr color_factor gray_factor(float alpha = 1.0) { return color_factor{0.5f, 0.5f, 0.5f, alpha}; }
	constexpr color_factor black_factor(float alpha = 1.0) { return color_factor{0.0f, 0.0f, 0.0f, alpha}; }
	constexpr color_factor red_factor(float alpha = 1.0) { return color_factor{1.0f, 0.0f, 0.0f, alpha}; }
	constexpr color_factor lime_factor(float alpha = 1.0) { return color_factor{0.0f, 1.0f, 0.0f, alpha}; }
	constexpr color_factor green_factor(float alpha = 1.0) { return color_factor{0.0f, 0.75f, 0.0f, alpha}; }
	constexpr color_factor blue_factor(float alpha = 1.0) { return color_factor{0.0f, 0.0f, 1.0f, alpha}; }
	constexpr color_factor cyan_factor(float alpha = 1.0) { return color_factor{0.0f, 1.0f, 1.0f, alpha}; }
	constexpr color_factor yellow_factor(float alpha = 1.0) { return color_factor{1.0f, 1.0f, 0.0f, alpha}; }
	constexpr color_factor magenta_factor(float alpha = 1.0) { return color_factor{1.0f, 0.0f, 1.0f, alpha}; }
	constexpr color_factor orange_factor(float alpha = 1.0) { return color_factor{1.0f, 0.5f, 0.0f, alpha}; }
	constexpr color_factor purple_factor(float alpha = 1.0) { return color_factor{0.5f, 0.0f, 0.5f, alpha}; }
	constexpr color_factor teal_factor(float alpha = 1.0) { return color_factor{0.0f, 0.5f, 0.5f, alpha}; }
	constexpr color_factor tan_factor(float alpha = 1.0) { return color_factor{0.75f, 0.625f, 0.5f, alpha}; }
	constexpr color_factor brown_factor(float alpha = 1.0) { return color_factor{0.5f, 0.25f, 0.0f, alpha}; }
}
