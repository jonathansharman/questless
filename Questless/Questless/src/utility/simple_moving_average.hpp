//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <array>

namespace ql {
	//! @tparam ValueType The quantity to average.
	//! @tparam N The maximum number of samples to average.
	template <typename ValueType, size_t Capacity>
	struct simple_moving_average {
		using value_type = ValueType;
		static constexpr size_t capacity = Capacity;

		static_assert(capacity > 0, "Capacity must be positive.");

		//! Adds @param value to the list, pushing out an old value if the buffer is full.
		void push(value_type value) {
			// Subtract oldest value.
			_sum -= _buffer[_idx];
			// Add new value.
			_buffer[_idx] = value;
			_sum += value;
			// Move index forward and wrap if needed.
			if (++_idx == capacity) { _idx = 0; }
			// Increase size, up to capacity.
			if (_size < capacity) { ++_size; }
		}

		//! Gets the current moving average.
		value_type get() const {
			return _sum / _size;
		}

	private:
		std::array<value_type, capacity> _buffer{};
		size_t _size = 0;
		size_t _idx = 0;
		value_type _sum = value_type(0);
	};
}

#include "doctest_wrapper/test.hpp"

TEST_CASE("[simple_moving_average] operations") {
	ql::simple_moving_average<double, 3> sma;
	CHECK_EQ(sma.capacity, 3);

	sma.push(1.0);
	CHECK(sma.get() == doctest::Approx(1.0));

	sma.push(2.0);
	CHECK(sma.get() == doctest::Approx(1.5));

	sma.push(3.0);
	CHECK(sma.get() == doctest::Approx(2.0));

	sma.push(4.0);
	CHECK(sma.get() == doctest::Approx(3.0));
}
