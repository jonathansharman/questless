/**
* @file    Quarterstaff.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Implements std::clamp from C++17. Implementation from http://en.cppreference.com/w/cpp/algorithm/clamp.
*/

#pragma once

/// @todo Remove and replace with std::clamp once implemented.

template<class T, class Compare>
constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp)
{
	return assert(!comp(hi, lo)),
		comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}

template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
	return ::clamp(v, lo, hi, std::less<>());
}
