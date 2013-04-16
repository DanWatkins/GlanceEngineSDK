/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: FrameImage.h
=================================================================================================*/
#include "Asset.h"

#ifndef _ge_FRAME_IMAGE_H
#define _ge_FRAME_IMAGE_H

namespace ge
{
	/*=============================================================================
	-- Frame asset to load reusable images for a SpriteFrame.
	=============================================================================*/	
	class FrameImage : public Asset
	{
	private:
		sf::Image mTop, mTopLeft, mTopRight;
		sf::Image mLeft, mRight, mCenter;
		sf::Image mBottom, mBottomLeft, mBottomRight;

	public:
		FrameImage(String id="") { _SetId(id); _SetAssetType(FRAME_IMAGE); }

		bool LoadFromFile(String filepath);
		bool PassImagesToSprite(SpriteFrame *spriteFrame);
	};
};

#endif

