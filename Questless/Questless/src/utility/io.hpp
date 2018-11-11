//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Utility functions for I/O tasks.

#pragma once

#include "id.hpp"
#include "lazy_bounded.hpp"
#include "nonnegative.hpp"
#include "static_bounded.hpp"

#include "cancel/quantity.hpp"
#include "units/space.hpp"

#include <cereal/archives/json.hpp>

#include <fstream>
#include <string>

namespace ql {
	//! The contents, as a string, of the file with filepath @p filepath.
	std::string contents_of_file(char const* filepath);

	template <typename T>
	void load_from_json(char const* filepath, T& value) {
		std::ifstream fin{filepath};
		cereal::JSONInputArchive archive{fin};
		archive(value);
	}
}
