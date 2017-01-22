/**
* @file    GameSeconds.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines GameSeconds as std::chrono::duration<double> and puts std::literals::chrono_literals into the global namespace.
*/

#pragma once

#include <chrono>

namespace units
{
	using GameSeconds = std::chrono::duration<double>;
}

using namespace std::literals::chrono_literals;
