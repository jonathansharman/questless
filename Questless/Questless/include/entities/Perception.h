//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/Bounded.h"

namespace questless
{
	//! Represents perception of an entity.
	struct Perception
	{
		//! Minimum possible perception level.
		static constexpr double minimum_level = 0.0;

		//! Maximum possible perception level.
		static constexpr double maximum_level = 100.0;

		//! Represents discrete, qualitative categories of which things can be perceived about an entity.
		enum class Category
			{ none   //!< Unperceived.
			, low    //!< Presence perceived.
			, medium //!< Type of entity perceived.
			, high   //!< Attributes such as health, mana, and equipped items perceived.
			, full   //!< All attributes perceived.
			};

		//! Minimum possible perception.
		static constexpr Perception minimum() { return Perception{minimum_level}; }

		//! Maximum possible perception.
		static constexpr Perception maximum() { return Perception{maximum_level}; }

		//! A quantitative measurement of the perception level, in the range [0, 100].
		Bounded<double, minimum_level, maximum_level> level;

		constexpr Perception() : level{0.0} {}
		constexpr Perception(double level) : level{level} {}
		constexpr Perception(Perception const&) = default;
		constexpr Perception(Perception&&) = default;
		constexpr Perception& operator =(Perception const&) = default;
		constexpr Perception& operator =(Perception&&) = default;

		//! The category of perception associated with the perception level.
		constexpr Category category() const
		{
			if (level > _high_full_threshold) {
				return Category::full;
			} else if (level > _medium_high_threshold) {
				return Category::high;
			} else if (level > _low_medium_threshold) {
				return Category::medium;
			} else if (level > _none_low_threshold) {
				return Category::low;
			} else {
				return Category::none;
			}
		}
	private:
		static constexpr double _none_low_threshold = 0.0;
		static constexpr double _low_medium_threshold = 25.0;
		static constexpr double _medium_high_threshold = 50.0;
		static constexpr double _high_full_threshold = 75.0;
	};
}
