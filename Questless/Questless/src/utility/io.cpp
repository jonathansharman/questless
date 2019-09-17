//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "io.hpp"

#include <fstream>
#include <sstream>

namespace ql {
	auto contents_of_file(char const* filepath) -> std::string {
		std::ifstream fin(filepath);
		std::stringstream ss;
		ss << fin.rdbuf();
		return ss.str();
	}
}
