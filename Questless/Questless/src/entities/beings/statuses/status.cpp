//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/status.hpp"
#include "utility/id.hpp"

namespace ql
{
	status::status(std::string name, int duration, std::optional<id<being>> source_id)
		: _name{std::move(name)}
		, _duration{duration}
		, _source_id{source_id}
	{}

	void status::apply(being&) {}

	void status::update(being& target)
	{
		--_duration;
		subupdate(target);
	}

	void status::expire(being&) {}

	void status::subupdate(being&) {}
}
