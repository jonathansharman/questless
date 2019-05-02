//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agents/action.hpp"
#include "utility/reference.hpp"

namespace ql {
	struct item;

	struct drop : action {
		drop(item& item) : _item{item} {}
		std::string name() const final {
			return "Drop";
		}
		complete perform(being& actor, cont cont) final;

	private:
		item& _item;
	};

	struct toss : action {
		toss(item& item) : _item{item} {}
		std::string name() const final {
			return "Throw";
		}
		complete perform(being& actor, cont cont) final;

	private:
		item& _item;
	};
}
