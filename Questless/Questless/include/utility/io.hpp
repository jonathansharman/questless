//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Utility functions for I/O tasks.

#pragma once

#include <string>

namespace ql
{
	//! The contents, as a string, of the file with filename @p filename.
	std::string contents_of_file(char const* filename);
}
