/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: SoundBuffer.h
  Created: 10/31/12 by Dan Watkins
=================================================================================================*/
#include "Asset.h"

#ifndef _ge_SOUNDBUFFER_H
#define _ge_SOUNDBUFFER_H

namespace ge
{
	/*=============================================================================
	-- SoundBuffer that can stream or simply load and play most audio formats.
	=============================================================================*/	
	class SoundBuffer : public Asset
	{
	private:
		sf::SoundBuffer mSoundBuffer;

	public:
		SoundBuffer(String id) { _SetId(id); _SetAssetType(SOUND_BUFFER); }

		sf::SoundBuffer *GetSoundBuffer() { return &mSoundBuffer; }
		bool LoadFromFile(String filepath) { return mSoundBuffer.LoadFromFile(filepath.GetStd()); };
	};
};

#endif

