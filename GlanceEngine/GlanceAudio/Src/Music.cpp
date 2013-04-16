/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Music.cpp
  Created: 10/31/12 by Dan Watkins
=================================================================================================*/
#include "Audio.h"

namespace ge
{
	/*=============================================================================
	-- Opens a connection between this and the specified music file for streaming.
	=============================================================================*/
	bool Music::OpenStreamFromFile(String filepath)
	{
		return mMusic.OpenFromFile(filepath.GetStd());
	}


	/*=============================================================================
	-- Returns the current playback state.
	=============================================================================*/
	AudioState Music::GetState()
	{
		switch (mMusic.GetStatus())
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