/**
* @file    Status.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Status class.
*/

#include "entities/beings/statuses/Status.h"
#include "entities/beings/Being.h"

namespace questless
{
	Status::Status(std::string name, int duration, Being* source)
		: _name{std::move(name)}
		, _duration{duration}
		, _source{source}
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
