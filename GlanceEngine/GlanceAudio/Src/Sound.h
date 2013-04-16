/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Sound.h
  Created: 10/30/12 by Dan Watkins
=================================================================================================*/
#include "Audio.h"

#ifndef _ge_SOUND_H
#define _ge_SOUND_H

namespace ge
{
	/*=============================================================================
	-- Used for playing short WAV sounds from a SoundBuffer. 
	=============================================================================*/	
	class Sound
	{
	private:
		sf::Sound mSound;

	public:
		void OpenStreamFromBuffer(SoundBuffer *buffer);

		void Play() { mSound.Play(); }
		void Pause() { mSound.Pause(); }
		void Stop() { mSound.Stop(); }

		void SetLooping(bool shouldLoop) { mSound.SetLoop(shouldLoop); }
		bool GetLooping() { return mSound.GetLoop(); }
		AudioState GetState();
		float GetPlayingOffset() { return mSound.GetPlayingOffset(); }
	};
};

#endif

