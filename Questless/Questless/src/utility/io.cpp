//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "utility/io.hpp"

#include <fstream>
#include <sstream>

#include "game.hpp"

namespace ql
{
	std::string contents_of_file(char const* filename)
	{
		std::ifstream fin(filename);
		std::stringstream ss;
		ss << fin.rdbuf();
		return ss.str();
	}
}
