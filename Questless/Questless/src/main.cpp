//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Runs an instance of the game Questless.

#ifndef _DEBUG
#define DOCTEST_CONFIG_DISABLE
#endif
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>
#undef near // Defined in minwindef.h (!)
#undef far // Defined in minwindef.h (!)

#include "game.hpp"

int main(int argc, char* argv[])
{
	int result = 0;

#ifdef _DEBUG
	doctest::Context context;
	context.setOption("no-breaks", false);
	context.setOption("success", false);
	context.applyCommandLine(argc, argv);
	result = context.run();
#else
	(void)argc; // Supress unused variable warning.
	(void)argv; // Supress unused variable warning.
#endif

	//try {
		ql::the_game().run();
	//} catch (std::exception& ex) {
	//	std::string message = ex.what();
	//	//! @todo Try-catch for exception for last-resort error reporting. (Should log the message or something.)
	//}
	return result;
}
