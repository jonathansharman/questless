//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/static_bounded.h"

namespace ql
{
	//! Represents perception of an entity.
	struct perception
	{
		//! Minimum possible perception level.
		static constexpr double minimum_level = 0.0;

		//! Maximum possible perception level.
		static constexpr double maximum_level = 100.0;

		//! Represents discrete, qualitative categories of which things can be perceived about an entity.
		enum class category
			{ none   //!< Unperceived.
			, low    //!< Presence perceived.
			, medium //!< Type of entity perceived.
			, high   //!< Attributes such as health, mana, and equipped items perceived.
			, full   //!< All attributes perceived.
			};

		//! Minimum possible perception.
		static constexpr perception minimum() { return perception{minimum_level}; }

		//! Maximum possible perception.
		static constexpr perception maximum() { return perception{maximum_level}; }

		//! A quantitative measurement of the perception level, in the range [0, 100].
		static_bounded<double, minimum_level, maximum_level> level;

		constexpr perception() : level{0.0} {}
		constexpr perception(double level) : level{level} {}
		constexpr perception(perception const&) = default;
		constexpr perception(perception&&) = default;
		constexpr perception& operator =(perception const&) = default;
		constexpr perception& operator =(perception&&) = default;

		//! The category of perception associated with the perception level.
		constexpr category category() const
		{
			if (level > _high_full_threshold) {
				return category::full;
			} else if (level > _medium_high_threshold) {
				return category::high;
			} else if (level > _low_medium_threshold) {
				return category::medium;
			} else if (level > _none_low_threshold) {
				return category::low;
			} else {
				return category::none;
			}
		}
	private:
		static constexpr double _none_low_threshold = 0.0;
		static constexpr double _low_medium_threshold = 25.0;
		static constexpr double _medium_high_threshold = 50.0;
		static constexpr double _high_full_threshold = 75.0;
	};
}
