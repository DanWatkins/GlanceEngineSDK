/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ButtonImage.cpp
  Created: 11/21/12 by Dan Watkins
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for ButtonImage.
		=============================================================================*/
		ButtonImage::ButtonImage()
		{
			_SetState(UP);
		}


		/*=============================================================================
		-- Initializes the ButtonImage after it has been added to the Root.
		   Automatically called from its corresponding Root::Create() method.
		=============================================================================*/
		bool ButtonImage::_PostInit()
		{
			//initialize 
			bool error = !mImage.Init(GetWindow(), GetRoot()->GetAssetManager()->GetImage(mImageName)->GetImage());
			mImage.SetColumns(2);
			mImage.SetIsSheet(true);
			mImage.SetFrameDimensions(mImage.GetWidth()/2, mImage.GetHeight());

			return !error;
		}


		/*=============================================================================
		-- Updates everything for the ButtonImage.
		=============================================================================*/
		void ButtonImage::_Update()
		{
			//if the button's state is DOWN, but MBL is up, then the putton should be released
			if (GetState() == DOWN  &&  GetRoot()->GetWindow()->GetInput()->GetButtonState(GMB_Left) == false)
				_SetState(RELEASED);
			else if (GetState() == RELEASED)
				_SetState(UP);
		}


		/*=============================================================================
		-- Draws the button depending on the state.
		=============================================================================*/
		void ButtonImage::_Draw()
		{
			if (GetState() == UP)
			{
				mImage.SetFrame(0);
				mImage.Draw();
			}
			else
			{
				mImage.SetFrame(1);
				mImage.Draw();
			}
		}

		/*=============================================================================
		-- Message handling.
		=============================================================================*/
		void ButtonImage::_SendMessage(Message message)
		{
			switch (message)
			{
			case MBL_DOWN_OVER:
				_SetState(DOWN);

			}
		}


		/*=============================================================================
		-- Updates the position and size of each asset.
		=============================================================================*/
		void ButtonImage::_UpdateAssets()
		{
			mImage.SetPos(GetScreenPos());
			mImage.SetSize(GetWidth()*2, GetHeight() );
		}


		/*=============================================================================
		-- Sets the caption of the text in the middle.
		=============================================================================*/
		void ButtonImage::_SetImageName(String imageName)
		{
			mImageName = imageName;
			//TODO should the images actually be reloaded?
		}
	};
};