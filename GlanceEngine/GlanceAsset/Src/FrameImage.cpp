/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: FrameImage.cpp
=================================================================================================*/
#include "Asset.h"

namespace ge
{
	/*=============================================================================
	-- Loads all nine images from file.
	=============================================================================*/	
	bool FrameImage::LoadFromFile(String filepath)
	{
		bool load = true;

		mCenter.LoadFromFile(filepath.GetStd()+"center.png");
		mTop.LoadFromFile(filepath.GetStd()+"top.png");
		mTopLeft.LoadFromFile(filepath.GetStd()+"top_left.png");
		mTopRight.LoadFromFile(filepath.GetStd()+"top_right.png");
		mLeft.LoadFromFile(filepath.GetStd()+"left.png");
		mRight.LoadFromFile(filepath.GetStd()+"right.png");
		mBottom.LoadFromFile(filepath.GetStd()+"bottom.png");
		mBottomLeft.LoadFromFile(filepath.GetStd()+"bottom_left.png");
		mBottomRight.LoadFromFile(filepath.GetStd()+"bottom_right.png");

		//set smoothing on off for all of them
		mCenter.SetSmooth(false);
		mTop.SetSmooth(false);
		mTopLeft.SetSmooth(false);
		mTopRight.SetSmooth(false);
		mLeft.SetSmooth(false);
		mRight.SetSmooth(false);
		mBottom.SetSmooth(false);
		mBottomLeft.SetSmooth(false);
		mBottomRight.SetSmooth(false);

		return load;
	}


	/*=============================================================================
	-- Loads the sprite frame with images from memory (stroed in this).
	=============================================================================*/	
	bool FrameImage::PassImagesToSprite(SpriteFrame *spriteFrame)
	{
		return spriteFrame->LoadImagesFromMemory(&mCenter, &mLeft, &mTop, &mRight, &mBottom, &mTopLeft,
											&mTopRight,&mBottomLeft, &mBottomRight);
	}


};