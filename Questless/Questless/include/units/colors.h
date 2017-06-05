//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines some mathematical constants under the namespace units::constants.

#pragma once

#include "ColorSpace.h"

namespace units::colors
{
	using Color = units::ColorSpace::Point;
	using ColorFactor = units::ColorSpace::Vector;

	constexpr Color clear(float red = 0.0, float green = 0.0, float blue = 0.0) { return Color{red, green, blue, 0.0f}; }
	constexpr Color white(float alpha = 1.0) { return Color{1.0f, 1.0f, 1.0f, alpha}; }
	constexpr Color silver(float alpha = 1.0) { return Color{0.75f, 0.75f, 0.75f, alpha}; }
	constexpr Color gray(float alpha = 1.0) { return Color{0.5f, 0.5f, 0.5f, alpha}; }
	constexpr Color black(float alpha = 1.0) { return Color{0.0f, 0.0f, 0.0f, alpha}; }
	constexpr Color red(float alpha = 1.0) { return Color{1.0f, 0.0f, 0.0f, alpha}; }
	constexpr Color lime(float alpha = 1.0) { return Color{0.0f, 1.0f, 0.0f, alpha}; }
	constexpr Color green(float alpha = 1.0) { return Color{0.0f, 0.75f, 0.0f, alpha}; }
	constexpr Color blue(float alpha = 1.0) { return Color{0.0f, 0.0f, 1.0f, alpha}; }
	constexpr Color cyan(float alpha = 1.0) { return Color{0.0f, 1.0f, 1.0f, alpha}; }
	constexpr Color yellow(float alpha = 1.0) { return Color{1.0f, 1.0f, 0.0f, alpha}; }
	constexpr Color magenta(float alpha = 1.0) { return Color{1.0f, 0.0f, 1.0f, alpha}; }
	constexpr Color orange(float alpha = 1.0) { return Color{1.0f, 0.5f, 0.0f, alpha}; }
	constexpr Color purple(float alpha = 1.0) { return Color{0.5f, 0.0f, 0.5f, alpha}; }
	constexpr Color teal(float alpha = 1.0) { return Color{0.0f, 0.5f, 0.5f, alpha}; }
	constexpr Color tan(float alpha = 1.0) { return Color{0.75f, 0.625f, 0.5f, alpha}; }
	constexpr Color brown(float alpha = 1.0) { return Color{0.5f, 0.25f, 0.0f, alpha}; }

	constexpr ColorFactor clear_factor(float alpha) { return ColorFactor{1.0f, 1.0f, 1.0f, alpha}; }
	constexpr ColorFactor white_factor(float alpha = 1.0) { return ColorFactor{1.0f, 1.0f, 1.0f, alpha}; }
	constexpr ColorFactor silver_factor(float alpha = 1.0) { return ColorFactor{0.75f, 0.75f, 0.75f, alpha}; }
	constexpr ColorFactor gray_factor(float alpha = 1.0) { return ColorFactor{0.5f, 0.5f, 0.5f, alpha}; }
	constexpr ColorFactor black_factor(float alpha = 1.0) { return ColorFactor{0.0f, 0.0f, 0.0f, alpha}; }
	constexpr ColorFactor red_factor(float alpha = 1.0) { return ColorFactor{1.0f, 0.0f, 0.0f, alpha}; }
	constexpr ColorFactor lime_factor(float alpha = 1.0) { return ColorFactor{0.0f, 1.0f, 0.0f, alpha}; }
	constexpr ColorFactor green_factor(float alpha = 1.0) { return ColorFactor{0.0f, 0.75f, 0.0f, alpha}; }
	constexpr ColorFactor blue_factor(float alpha = 1.0) { return ColorFactor{0.0f, 0.0f, 1.0f, alpha}; }
	constexpr ColorFactor cyan_factor(float alpha = 1.0) { return ColorFactor{0.0f, 1.0f, 1.0f, alpha}; }
	constexpr ColorFactor yellow_factor(float alpha = 1.0) { return ColorFactor{1.0f, 1.0f, 0.0f, alpha}; }
	constexpr ColorFactor magenta_factor(float alpha = 1.0) { return ColorFactor{1.0f, 0.0f, 1.0f, alpha}; }
	constexpr ColorFactor orange_factor(float alpha = 1.0) { return ColorFactor{1.0f, 0.5f, 0.0f, alpha}; }
	constexpr ColorFactor purple_factor(float alpha = 1.0) { return ColorFactor{0.5f, 0.0f, 0.5f, alpha}; }
	constexpr ColorFactor teal_factor(float alpha = 1.0) { return ColorFactor{0.0f, 0.5f, 0.5f, alpha}; }
	constexpr ColorFactor tan_factor(float alpha = 1.0) { return ColorFactor{0.75f, 0.625f, 0.5f, alpha}; }
	constexpr ColorFactor brown_factor(float alpha = 1.0) { return ColorFactor{0.5f, 0.25f, 0.0f, alpha}; }
}