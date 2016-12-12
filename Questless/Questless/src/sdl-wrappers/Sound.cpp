/**
* @file    Sound.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for Sound, a simple wrapper class around Mix_Chunk.
*/

#include "sdl-wrappers/Sound.h"

namespace sdl
{
	Sound::Sound(const std::string& filename)
	{
		_chunk = Mix_LoadWAV(filename.c_str());
		if (!_chunk) {
			throw std::runtime_error{("Failed to load sound \"" + filename + "\".").c_str()};
		}
	}

	Sound::Sound(Sound&& sound) : _chunk(sound._chunk), _channel(sound._channel)
	{
		sound._chunk = nullptr;
	}

	Sound::~Sound()
	{
		Mix_FreeChunk(_chunk);
	}

	Sound& Sound::operator =(Sound sound) &
	{
		swap(*this, sound);
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