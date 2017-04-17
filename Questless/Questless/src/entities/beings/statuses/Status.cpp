//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/statuses/Status.h"
#include "utility/Id.h"

namespace questless
{
	Status::Status(std::string name, int duration, std::optional<Id<Being>> source_id)
		: _name{std::move(name)}
		, _duration{duration}
		, _source_id{source_id}
	{}

	void Status::apply(Being&) {}

	void Status::update(Being& target)
	{
		--_duration;
		subupdate(target);
	}

	void Status::expire(Being&) {}

	void Status::subupdate(Being&) {}
}
