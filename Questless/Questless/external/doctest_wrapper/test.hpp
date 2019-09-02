//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Wraps the boilerplate required to write doctest tests.

#pragma once

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#include <doctest/doctest.h>
#undef near // Defined in minwindef.h (!)
#undef far // Defined in minwindef.h (!)
