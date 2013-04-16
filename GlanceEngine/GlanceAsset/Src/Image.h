/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Image.h
=================================================================================================*/
#include "Asset.h"

#ifndef _ge_IMAGE_H
#define _ge_IMAGE_H

namespace ge
{
	/*=============================================================================
	-- Image class to make an image be reusable.
	=============================================================================*/	
	class Image : public Asset
	{
	private:
		sf::Image mImage;

	public:
		Image(String id)
		{
			_SetId(id);
			_SetAssetType(IMAGE);
		}

		sf::Image *GetImage() { return &mImage; }
		bool LoadFromFile(String filepath) { return mImage.LoadFromFile(filepath.GetStd()); };
	};
};

#endif

