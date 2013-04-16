/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Music.h
  Created: 10/31/12 by Dan Watkins
=================================================================================================*/
#include "Audio.h"

#ifndef _ge_MUSIC_H
#define _ge_MUSIC_H

namespace ge
{
	/*=============================================================================
	-- Used for streaming long music files from disk (such as OGG) for playback.
	=============================================================================*/
	class Music
	{
	private:
		sf::Music mMusic;

	public:
		bool OpenStreamFromFile(String filepath);

		void Play() { mMusic.Play(); }
		void Pause() { mMusic.Pause(); }
		void Stop() { mMusic.Stop(); }

		void SetLooping(bool shouldLoop) { mMusic.SetLoop(shouldLoop); }
		bool GetLooping() { return mMusic.GetLoop(); }
		AudioState GetState();
		float GetPlayingOffset() { return mMusic.GetPlayingOffset(); }
	};
};

#endif

