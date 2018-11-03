//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "meta/quantity.hpp"

#include <chrono>

namespace ql {
	//! Wall time duration in seconds.
	using sec = meta::quantity<double, meta::unit_t<struct sec_tag>>;
	constexpr sec operator "" _s(long double value) { return sec{static_cast<double>(value)}; }

	//! Wall time duration in milliseconds.
	using msec = meta::quantity<double, meta::unit_t<struct msec_tag>>;
	constexpr msec operator "" _ms(long double value) { return msec{static_cast<double>(value)}; }

	//! Wall time rate in inverse seconds.
	using per_sec = meta::inverse_t<sec>;
	constexpr per_sec operator "" _hz(long double value) { return per_sec{static_cast<double>(value)}; }

	//! Wall time rate in inverse milliseconds.
	using per_msec = meta::inverse_t<msec>;
	constexpr per_msec operator "" _khz(long double value) { return per_msec{static_cast<double>(value)}; }

	constexpr auto ms_per_sec = 1000.0_ms / 1.0_s;
	constexpr auto sec_per_msec = 1.0_s / 1000.0_ms;

	//! The clock to use for wall time.
	using clock = std::chrono::steady_clock;

	//! The game's frames per second.
	static constexpr auto target_frame_rate = 60.0_hz;

	//! The duration of a single frame; inverse of FPS.
	static constexpr auto target_frame_duration = 1.0 / target_frame_rate;

	//! Converts from a seconds quantity to std::chrono::seconds.
	constexpr auto to_chrono_seconds(sec sec) { return std::chrono::duration<double>(sec.value); }

	//! Converts from std::chrono::seconds to a seconds quantity.
	constexpr auto to_sec_quantity(std::chrono::duration<double> duration) { return sec{duration.count()}; }
}
