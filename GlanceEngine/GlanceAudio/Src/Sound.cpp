/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Sound.cpp
  Created: 10/30/12 by Dan Watkins
=================================================================================================*/
#include "Audio.h"

namespace ge
{
	/*=============================================================================
	-- Opens a connection between this and the specified SoundBuffer for playback.
	=============================================================================*/	
	void Sound::OpenStreamFromBuffer(SoundBuffer *buffer)
	{
		mSound.SetBuffer(*buffer->GetSoundBuffer());
	}


	/*=============================================================================
	-- Returns the current playback state.
	=============================================================================*/
	AudioState Sound::GetState()
	{
		switch (mSound.GetStatus())
		{
		case sf::Sound::Playing:
			return PLAYING;
		case sf::Sound::Paused:
			return STOPPED;
		case sf::Sound::Stopped:
			return PAUSED;
		default:
			return STOPPED;
		}
	}
};