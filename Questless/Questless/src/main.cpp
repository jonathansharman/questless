/**
* @file    main.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Creates and runs an instance of the game Questless.
*/

#ifdef _DEBUG
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#endif

#include "Game.h"

int main(int argc, char* argv[])
{
	int ret = 0;

#ifdef _DEBUG
	doctest::Context context;
	context.setOption("no-breaks", true);
	context.setOption("success", false);
	context.applyCommandLine(argc, argv);
	ret = context.run();
#endif

	//try {
		questless::Game game(false);
		game.run();
	//} catch (std::exception& ex) {
	//	string message = ex.what();
	//	/// @todo Try-catch for exception for last-resort error reporting. (Should log the message or something.)
	//}
	return ret;
}
