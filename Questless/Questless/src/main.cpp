#include "Game.h"

int main(int argc, char* argv[])
{
	/// @todo This is to shut up the compiler warnings for unused variables.
	int x = argc;
	argc = x;
	char** y = argv;
	argv = y;

//	try {
		questless::Game game(false);
		//Game game(true);
		game.run();
//	} catch (std::exception& ex) {
//		string message = ex.what();
//		/// @todo Try-catch for exception for last-resort error reporting. (Should log the message or something.)
//	}

	return 0;
}
