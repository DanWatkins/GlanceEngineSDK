/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Audio.h
  Created: 10/31/12 by Dan Watkins
=================================================================================================*/
#include "../../GlanceAsset/Src/Asset.h"

#ifndef _ge_AUDIO_H
#define _ge_AUDIO_H

namespace ge
{
	/*=============================================================================
	-- Enumerated states for types of audio playback.
	=============================================================================*/
	enum AudioState
	{
		PLAYING,
		PAUSED,
		STOPPED
	};
};

#include "Sound.h"
#include "Music.h"

#endif

