//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Defines an UNREACHABLE macro, implementing http://open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0627r0.pdf.

#pragma once

#ifdef _MSC_VER
#define UNREACHABLE __assume(false)
#else
#define UNREACHABLE __builtin_unreachable()
#endif
