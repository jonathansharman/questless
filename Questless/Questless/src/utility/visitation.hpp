//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Utility functions for making variant visitation nicer.

#pragma once

#include <variant>

namespace ql {
	//! Creates an overloaded function object from the given function types.
	template <typename... Funcs>
	struct overloaded : Funcs... {
		using Funcs::operator()...;
	};

	//! Creates an overloaded function object from the given function objects.
	template <typename... Funcs>
	overloaded(Funcs...)->overloaded<Funcs...>;

	//! Visits the variant @p v using the handlers @p handlers.
	//! @note Credit to Nikolai Wuttke, "std::variant and the power of pattern matching".
	template <typename Variant, typename... Handlers>
	auto match(Variant&& v, Handlers&&... handlers) {
		return std::visit(overloaded{std::forward<Handlers>(handlers)...}, std::forward<Variant>(v));
	}
}
