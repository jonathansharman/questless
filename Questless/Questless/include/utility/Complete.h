/**
* @file    Complete.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

namespace questless
{
	/// Trivial type to use as the return type from functions with callbacks that don't need to return anything.
	/// The purpose of returning a Complete value, rather than void, from a function that takes a callback is
	/// to enforce either (1) calling the callback at the end of the function or (2) effectively documenting
	/// the decision not to do so in that return path. When returning void form a function with a callback,
	/// it's too easy to simply forget to invoke the callback or to invoke it without returning.
	////
	struct Complete {};
}
