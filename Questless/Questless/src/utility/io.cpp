//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "io.hpp"

#include <fstream>
#include <sstream>

namespace ql {
	std::string contents_of_file(char const* filepath) {
		std::ifstream fin(filepath);
		std::stringstream ss;
		ss << fin.rdbuf();
		return ss.str();
	}
}
