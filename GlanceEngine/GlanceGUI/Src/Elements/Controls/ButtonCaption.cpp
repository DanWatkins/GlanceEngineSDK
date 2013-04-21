/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ButtonCaption.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for ButtonCaption.
		=============================================================================*/
		ButtonCaption::ButtonCaption()
		{
			_SetState(UP);
			mCaption = "";
		}


		/*=============================================================================
		-- Initializes the ButtonCaption after it has been added to the Root.
		   Automatically called from its corresponding Root::Create() method.
		=============================================================================*/
		bool ButtonCaption::_PostInit()
		{
			bool error = false;

			//initialize 
			if (!mImgUp.Init( GetWidth(), GetHeight(), GetWindow()) ||
				!((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("button_up"))->PassImagesToSprite(&mImgUp) ||
				!mImgDown.Init( GetWidth(), GetHeight(), ToString(DIR_GUI)+GUI_DIR_BUTTON+"down/", GetWindow()) ||
				!((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("button_down"))->PassImagesToSprite(&mImgDown)
			)
				error = true;

			//configure the integrated text element
			GetRoot()->CreateText(IEI_BUTTONCAPTION_TEXT, GetId(), Vector2D<int>(), mCaption);
			SetCaption(mCaption);			

			return !error;
		}


		/*=============================================================================
		-- Updates everything for the ButtonCaption.
		=============================================================================*/
		void ButtonCaption::_Update()
		{
			//if the button's state is DOWN, but MBL is up, then the putton should be released
			if (GetState() == DOWN  &&  GetRoot()->GetWindow()->GetInput()->GetButtonState(GMB_Left) == false)
				_SetState(RELEASED);
			else if (GetState() == RELEASED)
				_SetState(UP);
		}


		/*=============================================================================
		-- Draws the button depending on the state and the caption overlay.
		=============================================================================*/
		void ButtonCaption::_Draw()
		{
			if (GetState() == UP)
				mImgUp.Draw();
			else
				mImgDown.Draw();
		}

		/*=============================================================================
		-- Message handling.
		=============================================================================*/
		void ButtonCaption::_SendMessage(Message message)
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
		void ButtonCaption::_UpdateAssets()
		{
			mImgUp.SetPos(GetScreenPos());
			mImgUp.SetSize( GetWidth(), GetHeight() );
			mImgUp.ResetPos();
			mImgUp.ResetSize();

			mImgDown.SetPos(GetScreenPos());
			mImgDown.SetSize( GetWidth(), GetHeight() );
			mImgDown.ResetPos();
			mImgDown.ResetSize();
		}


		/*=============================================================================
		-- Sets the caption of the text in the middle.
		=============================================================================*/
		void ButtonCaption::SetCaption(String caption)
		{
			mCaption = caption;
			WeakPtr<Text> text = DynamicPtrCast<Text>(GetChild(IEI_BUTTONCAPTION_TEXT).lock());

			if (!text.expired())
			{
				text.lock()->SetText(caption);
				Font *font = text.lock()->GetFont();

				unsigned width = (unsigned)font->GetTextLengthPx(mCaption.GetStd());
				unsigned height = (unsigned)font->GetCharHeightPx();


				//clip any characters that don't fit inside from the text element
				while (width > GetWidth())
				{
					mCaption.Remove(mCaption.Size()-1,mCaption.Size());
					SetCaption(mCaption);
					width = (unsigned)font->GetTextLengthPx(mCaption.GetStd());
				}

				//properly center the text
				text.lock()->SetRelPos( Vector2D<int>((GetWidth()-width)/2, (GetHeight()-height)/2) );
			}
		}
	};
};