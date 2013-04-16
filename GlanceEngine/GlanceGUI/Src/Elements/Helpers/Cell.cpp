/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Cell.cpp
  Created: 2/2/13 by Dan Watkins
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Initializes the Cell after it has been added to the Root. Automatically
		   called from its corresponding Root::Create() method.
		=============================================================================*/
		bool Cell::_PostInit()
		{
			mBackgroundImage.SetWindow(GetWindow());

			return true;
		}


		/*=============================================================================
		-- Draws the Cell.
		=============================================================================*/
		void Cell::_Draw()
		{
			mBackgroundImage.Draw();
		}


		/*=============================================================================
		-- Updates the position and size of each asset.
		=============================================================================*/
		void Cell::_UpdateAssets()
		{
			mBackgroundImage.SetPos(GetScreenPos());
			mBackgroundImage.SetSize(GetWidth(), GetHeight());
		}


		/*=============================================================================
		-- Sets the background image for the entire cell.
		=============================================================================*/
		void Cell::SetBackgroundImage(String name)
		{
			mBackgroundImage.SetImage(*GetRoot()->GetAssetManager()->GetImage(name)->GetImage());
			_UpdateAssets();
		}
	};
};