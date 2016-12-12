/**
* @file    Sound.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for Sound, a simple wrapper class around Mix_Chunk.
*/

#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <algorithm>
#include <memory>

#include <SDL.h>
#include <SDL_mixer.h>

namespace sdl
{
	class Sound
	{
	public:
		using ptr = std::unique_ptr<Sound>;

		/// @param filename The WAV file from which to load the sound.
		Sound(const std::string& filename);

		/// Disable copy constructor.
		Sound(const Sound& sound) = delete;

		/// Move constructor.
		Sound(Sound&& sound);

		/// @return A unique sound pointer constructed with the given arguments.
		template <typename... Args>
		static ptr make(Args&&... args) { return std::make_unique<Sound>(std::forward<Args>(args)...); }

		~Sound();

		Sound& operator =(Sound sound) &;
	
		friend void swap(Sound& first, Sound& second);

		/// @return The internal Mix_Chunk pointer.
		Mix_Chunk* const sdl_ptr() { return _chunk; }

		/// Plays the sound.
		/// @param loop The number of times to loop the sound. 0 for no loop.
		void play(int loop = 0) const;

		/// Stops the sound.
		void stop() const { Mix_Pause(_channel); }
	private:
		Mix_Chunk* _chunk;
		mutable int _channel;
	};
}

#endif
