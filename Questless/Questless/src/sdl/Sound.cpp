//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/Sound.h"

namespace sdl
{
	Sound::Sound(char const* filename)
	{
		_chunk = Mix_LoadWAV(filename);
		if (!_chunk) {
			throw std::runtime_error{("Failed to load sound \"" + std::string{filename} + "\".").c_str()};
		}
	}

	Sound::Sound(Sound&& that) : _chunk(that._chunk), _channel(that._channel)
	{
		that._chunk = nullptr;
	}

	Sound::~Sound()
	{
		Mix_FreeChunk(_chunk);
	}

	Sound& Sound::operator =(Sound that) &
	{
		swap(*this, that);
		return *this;
	}

	void swap(Sound& first, Sound& second)
	{
		std::swap(first._chunk, second._chunk);
		std::swap(first._channel, second._channel);
	}

	void Sound::play(int loop) const
	{
		_channel = Mix_PlayChannel(-1, _chunk, loop);
	}
}
