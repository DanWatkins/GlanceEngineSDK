/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Asset.h
=================================================================================================*/
#include "../../GlanceRender2D/Src/Render2D.h"

#ifndef _ge_ASSET_H
#define _ge_ASSET_H

namespace ge
{
	enum AssetType
	{
		UNDEFINED,
		IMAGE,
		FRAME_IMAGE,
		SOUND_BUFFER
	};


	/*=============================================================================
	-- Asset base class.
	=============================================================================*/
	class Asset
	{
	private:
		String mId;			//unique id of the asset
		AssetType mAssetType;

	protected:
		void _SetId(String id) { mId = id; }
		void _SetAssetType(AssetType type) { mAssetType = type; }

	public:
		Asset() { _SetAssetType(UNDEFINED); }
		~Asset() {}

		String GetId() { return mId; }
		AssetType GetAssetType() { return mAssetType; }
		virtual bool LoadFromFile(String filepath) = 0;
	};
};

#include "Image.h"
#include "SoundBuffer.h"
#include "FrameImage.h"
#include "AssetManager.h"


#endif

