#include "Game.h"

int main(int argc, char* argv[])
{
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
