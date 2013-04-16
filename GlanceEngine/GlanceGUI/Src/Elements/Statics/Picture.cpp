/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Picture.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for Image.
		=============================================================================*/
		Picture::Picture()
		{
			mFilepath = "";
			SetCanFocus(false);
		}


		/*=============================================================================
		-- Initializes the GroupBox after it has been added to the Root. Automatically
		   called from its corresponding Root::Create() method.
		=============================================================================*/
		bool Picture::_PostInit()
		{
			mImg.SetWindow(this->GetWindow());

			if (mFilepath != "")
				return mImg.LoadImageFromFile(mFilepath.GetStd());

			return true;
		}


		/*=============================================================================
		-- Draws the icon.
		=============================================================================*/
		void Picture::_Draw()
		{
			mImg.Draw();
		}


		/*=============================================================================
		-- Updates the scalar metrics of any assets.
		=============================================================================*/
		void Picture::_UpdateAssets()
		{
			mImg.SetPos( GetScreenPos().x, GetScreenPos().y );
			mImg.SetSize(GetWidth(), GetHeight());
		}


		/*=============================================================================
		-- Sets the internal image of the sprite representing the picture.
		=============================================================================*/
		void Picture::SetImage(Image *image)
		{
			mImg.SetImage(*image->GetImage());
			_UpdateAssets();
		}
	};
};