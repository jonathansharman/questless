//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines macros to cut down on boilerplate when writing std::variant visitors.

#pragma once

#include "utility.hpp"

#define SWITCH_TYPE(arg) using ACTUAL_VARIANT_TYPE = std::decay_t<decltype(arg)>;

#define MATCH_TYPE(type) if constexpr (std::is_same_v<ACTUAL_VARIANT_TYPE, type>)
