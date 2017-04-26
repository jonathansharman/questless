//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <algorithm>
#include <memory>

#include <SDL.h>
#include <SDL_mixer.h>

namespace sdl
{
	//! A simple wrapper around Mix_Chunk.
	class Sound
	{
	public:
		//! @param filename The WAV file from which to load the sound.
		Sound(char const* filename);
		Sound(Sound const&) = delete;
		Sound(Sound&& that);

		~Sound();

		Sound& operator =(Sound that) &;
	
		friend void swap(Sound& first, Sound& second);

		//! The internal Mix_Chunk pointer.
		Mix_Chunk* const sdl_ptr() { return _chunk; }

		//! Plays the sound.
		//! @param loop The number of times to loop the sound. 0 for no loop.
		void play(int loop = 0) const;

		//! Stops the sound.
		void stop() const { Mix_Pause(_channel); }
	private:
		Mix_Chunk* _chunk;
		mutable int _channel;
	};
}
