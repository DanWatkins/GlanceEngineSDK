/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Panel.cpp
  Created: 4/21/2013 @ 19:38 UTC-6
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for GroupBox.
		=============================================================================*/
		Panel::Panel()
		{
			SetCanFocus(false);
		}


		/*=============================================================================
		-- Initializes the GroupBox after it has been added to the Root. Automatically
		   called from its corresponding Root::Create() method.
		=============================================================================*/
		bool Panel::_PostInit()
		{
			//load border sprite frame from memory
			mImgPanel.Init( GetWidth(), GetHeight(), GetRoot()->GetWindow() );
			((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("groupbox"))->PassImagesToSprite(&mImgPanel);

			return true;
		}


		/*=============================================================================
		-- Draws the GroupBox.
		=============================================================================*/
		void Panel::_Draw()
		{
			mImgPanel.Draw();
		}


		/*=============================================================================
		-- Updates the scalar metrics of any assets.
		=============================================================================*/
		void Panel::_UpdateAssets()
		{
			mImgPanel.SetPos(GetScreenPos());
			mImgPanel.SetSize( GetWidth(), GetHeight() );
			mImgPanel.ResetPos();
			mImgPanel.ResetSize();
		}
	};
};