/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: EditBox.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Forces the caret to be shown and restarts the timer
		=============================================================================*/
		void EditBox::_ForceShowCaretReset()
		{
			mCaretIsShowing = true;
			mCaretTimer.Start();
		}

		/*=============================================================================
		-- Constructor for EditBox.
		=============================================================================*/
		EditBox::EditBox()
		{
			mCaretIsShowing = true;
		}


		/*=============================================================================
		-- Initializes the GroupBox after it has been added to the Root. Automatically
		   called from its corresponding Root::Create() method.
		=============================================================================*/
		bool EditBox::_PostInit()
		{
			//load the images
			mImgCaret.Init(GetWindow(), ((Image*)GetRoot()->GetAssetManager()->GetAsset(GAID_CARET))->GetImage());
			mImgBorder.Init( GetWidth(), GetHeight(), GetWindow() );
			((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("edit_border"))->PassImagesToSprite(&mImgBorder);

			//create the text element and link @mText with it
			if (!GetRoot()->CreateText(IEI_EDITBOX_TEXT, GetId(), Vector2D<int>(), "").expired())
				mText = DynamicPtrCast<Text>(GetChild(IEI_EDITBOX_TEXT).lock());

			//set the size of the caret image
			mImgCaret.SetSize(1, GetRoot()->GetDefaultFont()->GetCharHeightPx());

			//set the caption to the caption (just in case it was set before Init() was called)
			SetCaption(mCaption);

			return true;
		}


		/*=============================================================================
		-- Updates the EditBox.
		=============================================================================*/
		void EditBox::_Update()
		{
			if (GetHasFocus())
			{
				String newText = mText.lock()->GetText();
				newText.PushBack(GetWindow()->GetInput()->GetTextEntered());

				//don't set the text directly or else it won't be restricted properly
				SetCaption(newText);
			}

			_UpdateAssets();
		}


		/*=============================================================================
		-- Draws the EditBox.
		=============================================================================*/
		void EditBox::_Draw()
		{
			mImgBorder.Draw();

			if (GetHasFocus())
			{
				if (mCaretIsShowing)
					mImgCaret.Draw();

				//switch if the caret is showing if enough time has passed
				if (mCaretTimer.GetTimeAndEnd() > GUI_CARET_DELAY)
				{
					mCaretIsShowing = !mCaretIsShowing;
					mCaretTimer.Start();
				}
			}
		}


		/*=============================================================================
		-- Updates the scalar metrics of any assets.
		=============================================================================*/
		void EditBox::_UpdateAssets()
		{
			mImgBorder.SetPos(GetScreenPos());
			mImgBorder.ResetPos();


			//position the caret
			if (!mText.expired())
			{
				unsigned width = GetRoot()->GetDefaultFont()->GetTextLengthPx(mText.lock()->GetText());
				mImgCaret.SetPos( GetScreenPos().x+width+GUI_EDIT_TEXT_PAD, GetScreenPos().y+GUI_EDIT_TEXT_PAD );
			}
		}


		/*=============================================================================
		-- Sets the text and ensures it fits.
		=============================================================================*/
		void EditBox::SetCaption(String caption)
		{
			mCaption = caption;

			if (!mText.expired())
			{
				//always apply operations
				mCaption.ApplyOperations();

				//truncate @caption until it fits
				while (mText.lock()->GetFont()->GetTextLengthPx(mCaption) > GetWidth()-GUI_EDIT_TEXT_PAD*2.0f)
					mCaption.RemoveLast(1);

				mText.lock()->SetText(mCaption);

				//reset the text rel pos
				unsigned height = mText.lock()->GetFont()->GetCharHeightPx();
				Vector2D<int> offset;
				offset.x = GUI_EDIT_TEXT_PAD;
				offset.y = (GetHeight()-height)/2;
				mText.lock()->SetRelPos(offset);

				_UpdateAssets();
			}
		}


		/*=============================================================================
		-- Message handler.
		=============================================================================*/
		void EditBox::_SendMessage(Message message)
		{
			if (message == HAS_FOCUS)
				mCaretTimer.Start();
		}
	};
};