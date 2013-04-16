/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: AssetManager.h
=================================================================================================*/
#include "Asset.h"

#ifndef _ge_ASSET_MANAGER_H
#define _ge_ASSET_MANAGER_H

namespace ge
{
	/*=============================================================================
	-- Keeps track of every asset and lets them be reused.
	=============================================================================*/	
	class AssetManager
	{
	private:
		std::vector< Asset *> mAssets;			//container for every asset
		std::vector< String > mImageFormats;	//list of image formats that the manager attempts to load


	public:
		AssetManager();
		~AssetManager() {}

		//general
		Asset *GetAsset(String id);
		Image *GetImage(String id);

		bool LoadAssetsFromDirectory(String path);
		void AddImageFormat(String extension);
		bool ValidImageFormat(String extension);
		void AddAsset(Asset *asset);
	};
};

#endif

