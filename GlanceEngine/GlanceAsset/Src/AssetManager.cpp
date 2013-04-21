/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: AssetManager.cpp
=================================================================================================*/
#include "Asset.h"

namespace ge
{
	/*=============================================================================
	-- Constructor for AssetManager.
	=============================================================================*/
	AssetManager::AssetManager()
	{
		AddImageFormat(".bmp");
		AddImageFormat(".png");
		AddImageFormat(".jpg");
		AddImageFormat(".gif");
		AddImageFormat(".tif");
		AddImageFormat(".tga");
	}


	/*=============================================================================
	-- Adds an asset to the asset list if an asset with the same id doesn't exist
	   already.
	=============================================================================*/
	void AssetManager::AddAsset(Asset *asset)
	{
		if (asset != NULL)
		{
			if (GetAsset(asset->GetId()) == NULL)
				mAssets.push_back(asset);
		}
	}


	/*=============================================================================
	-- Returns a pointer to an Asset with the specified id.
	=============================================================================*/
	Asset *AssetManager::GetAsset(String id)
	{
		std::vector< Asset *>::iterator iter = mAssets.begin();
		while (iter != mAssets.end())
		{
			if ( (*iter)->GetId() == id )
				return *iter;

			iter++;
		}

		gDebug.Error(String()+"Asset \""+id+"\" could not be found in AssetManager instance at 0x"+(int)this);

		return NULL;
	}


	/*=============================================================================
	-- Returns a pointer to an Image asset with the sspecified id. If the Asset
       with the id is not an Image, NULL will be returned.
	=============================================================================*/
	Image *AssetManager::GetImage(String id)
	{
		Asset *asset = GetAsset(id);
		if (asset)
		{
			if (typeid(*GetAsset(id)) == typeid(Image))
				return (Image*)GetAsset(id);

			gDebug.Error("Asset \""+id+"\" is not an Image object in AssetManager instance at 0x"+this);
		}
		

		return NULL;
	}


	/*=============================================================================
	-- Loads every asset in the @pdirectory. It does not search sub-directories.
	=============================================================================*/
	bool AssetManager::LoadAssetsFromDirectory(String path)
	{
		io::Directory directory;
		io::DirectoryListing listing;
		directory.SetPath(path.GetStd());

		//make sure this is not a file, and then get the directory contents
		if (io::IsFile(path) == false)
			listing = directory.ExportDirectoryListing();

		io::DirectoryListing::iterator iter = listing.begin();
		while (iter != listing.end())
		{
			if (ValidImageFormat(io::GetExtension(*iter)))
			{
				String id = io::ShortenPath( (*iter));
				io::RemoveExtension(id);

				Image *image = new Image(id);
				image->LoadFromFile(*iter);
				mAssets.push_back(image);
			}

			iter++;
		}

		return true;
	}


	/*=============================================================================
	-- Adds an image format to the list of supported image formats. By supported,
	   it means what it considers to be an image. The method accepts a parameter
	   in the form of a String representing the extension of the image format.
	   The period prefix must be including. Here is an example of how to call this
	   method: manager->AddImageFormat(".bmp");
	=============================================================================*/
	void AssetManager::AddImageFormat(String extension)
	{
		//make sure the extension is not already in the list
		std::vector< String >::iterator iter = mImageFormats.begin();
		while (iter != mImageFormats.end())
		{
			if (*iter == extension)
				return;

			iter++;
		}

		//add the extension to the image format list
		mImageFormats.push_back(extension);
	}


	/*=============================================================================
	-- Returns true if the specified image format is in the list of image formats.
	   Include the period prefix. Here is an exaple call:
	   manager->ValidImageFormat(".bmp");
	=============================================================================*/
	bool AssetManager::ValidImageFormat(String extension)
	{
		std::vector< String >::iterator iter = mImageFormats.begin();
		while (iter != mImageFormats.end())
		{
			if (*iter == extension)
				return true;

			iter++;
		}

		return false;
	}
};