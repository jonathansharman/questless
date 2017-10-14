//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/sound.h"

namespace sdl
{
	sound::sound(char const* filename)
	{
		_chunk = Mix_LoadWAV(filename);
		if (!_chunk) {
			throw std::runtime_error{("Failed to load sound \"" + std::string{filename} + "\".").c_str()};
		}
	}

	sound::sound(sound&& that) : _chunk(that._chunk), _channel(that._channel)
	{
		that._chunk = nullptr;
	}

	sound::~sound()
	{
		Mix_FreeChunk(_chunk);
	}

	sound& sound::operator =(sound that) &
	{
		swap(*this, that);
		return *this;
	}

	void swap(sound& first, sound& second)
	{
		std::swap(first._chunk, second._chunk);
		std::swap(first._channel, second._channel);
	}

	void sound::play(int loop) const
	{
		_channel = Mix_PlayChannel(-1, _chunk, loop);
	}
}
