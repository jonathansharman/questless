/**
* @file    main.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Runs an instance of the game Questless.
*/

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#include "Game.h"

int main(int argc, char* argv[])
{
	int result = 0;

#ifdef _DEBUG
	doctest::Context context;
	context.setOption("no-breaks", true);
	context.setOption("success", false);
	context.applyCommandLine(argc, argv);
	result = context.run();
#endif

	//try {
		questless::game().run();
	//} catch (std::exception& ex) {
	//	string message = ex.what();
	//	/// @todo Try-catch for exception for last-resort error reporting. (Should log the message or something.)
	//}
	return result;
}
