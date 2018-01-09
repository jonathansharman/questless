//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines some mathematical constants under the namespace units::constants.

#pragma once

#include "color_space.hpp"

namespace units::colors
{
	using color = units::color_space::point;
	using color_vector = units::color_space::vector;

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

	constexpr color_vector clear_vector(float alpha) { return color_vector{1.0f, 1.0f, 1.0f, alpha}; }
	constexpr color_vector white_vector(float alpha = 1.0) { return color_vector{1.0f, 1.0f, 1.0f, alpha}; }
	constexpr color_vector silver_vector(float alpha = 1.0) { return color_vector{0.75f, 0.75f, 0.75f, alpha}; }
	constexpr color_vector gray_vector(float alpha = 1.0) { return color_vector{0.5f, 0.5f, 0.5f, alpha}; }
	constexpr color_vector black_vector(float alpha = 1.0) { return color_vector{0.0f, 0.0f, 0.0f, alpha}; }
	constexpr color_vector red_vector(float alpha = 1.0) { return color_vector{1.0f, 0.0f, 0.0f, alpha}; }
	constexpr color_vector lime_vector(float alpha = 1.0) { return color_vector{0.0f, 1.0f, 0.0f, alpha}; }
	constexpr color_vector green_vector(float alpha = 1.0) { return color_vector{0.0f, 0.75f, 0.0f, alpha}; }
	constexpr color_vector blue_vector(float alpha = 1.0) { return color_vector{0.0f, 0.0f, 1.0f, alpha}; }
	constexpr color_vector cyan_vector(float alpha = 1.0) { return color_vector{0.0f, 1.0f, 1.0f, alpha}; }
	constexpr color_vector yellow_vector(float alpha = 1.0) { return color_vector{1.0f, 1.0f, 0.0f, alpha}; }
	constexpr color_vector magenta_vector(float alpha = 1.0) { return color_vector{1.0f, 0.0f, 1.0f, alpha}; }
	constexpr color_vector orange_vector(float alpha = 1.0) { return color_vector{1.0f, 0.5f, 0.0f, alpha}; }
	constexpr color_vector purple_vector(float alpha = 1.0) { return color_vector{0.5f, 0.0f, 0.5f, alpha}; }
	constexpr color_vector teal_vector(float alpha = 1.0) { return color_vector{0.0f, 0.5f, 0.5f, alpha}; }
	constexpr color_vector tan_vector(float alpha = 1.0) { return color_vector{0.75f, 0.625f, 0.5f, alpha}; }
	constexpr color_vector brown_vector(float alpha = 1.0) { return color_vector{0.5f, 0.25f, 0.0f, alpha}; }
}
