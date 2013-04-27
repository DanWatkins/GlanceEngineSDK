/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: DialogBox.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Initializes the DialogBox after it has been added to the Root. Automatically
		   called from its corresponding Root::Create() method.
		=============================================================================*/
		bool DialogBox::_PostInit()
		{
			//create the caption text element
			GetRoot()->CreateText(IEI_DIALOGBOX_TEXT, GetId(), Vector2D<int>(5, 5), "");
			SetCaption(mCaption);

			//create the drager element inside the titlebar area
			mTitleBarDrager = GetRoot()->CreateDrager(IEI_DIALOGBOX_DRAGER, GetId(), Vector2D<int>(), GetWidth(), TITLE_BAR_HEIGHT, true, true);

			mCloseButton = GetRoot()->CreateButtonImage(IEI_DIALOGBOX_CLOSE_BUTTON, GetId(),
										Vector2D<int>(GetWidth()-CLOSE_BUTTON_SIZE-4,4),
										CLOSE_BUTTON_SIZE, CLOSE_BUTTON_SIZE, "x_button.bmp");

			//load the sprite frames
			mImgTitleBar.Init(GetWidth(), TITLE_BAR_HEIGHT, GetWindow(), true);
			((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("dialog_title_bar"))->PassImagesToSprite(&mImgTitleBar);
			mImgClientArea.Init(GetWidth(), GetHeight()-TITLE_BAR_HEIGHT, GetWindow(), true);
			((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("dialog_client_area"))->PassImagesToSprite(&mImgClientArea);

			return true;
		}


		void DialogBox::_Update()
		{
			//was the close button pressed (released)?
			if (mCloseButton.lock()->GetState() == State::RELEASED)
			{
				WeakPtr<Element> parent = GetParent();
				if (!parent.expired())
					_GetAsWeakPtr().lock()->ScheduleToBeRemoved();
				else
					_GetAsWeakPtr().lock()->ScheduleToBeRemoved();
			}
		}


		/*=============================================================================
		-- Draws the button depending on the state and the caption overlay.
		=============================================================================*/
		void DialogBox::_Draw()
		{
			mImgClientArea.Draw();
			mImgTitleBar.Draw();
		}


		/*=============================================================================
		-- Updates the position and size of each asset.
		=============================================================================*/
		void DialogBox::_UpdateAssets()
		{
			mImgTitleBar.SetSize(GetWidth(), TITLE_BAR_HEIGHT);
			mImgTitleBar.SetPos(GetScreenPos());
			mImgClientArea.SetSize(GetWidth(), GetHeight()-TITLE_BAR_HEIGHT);
			mImgClientArea.SetPos(GetScreenPos().x, GetScreenPos().y+TITLE_BAR_HEIGHT);

			WeakPtr<Text> text = DynamicPtrCast<Text>(GetChild(-2).lock());
			if (!text.expired())
				text.lock()->SetRelPos( Vector2D<int>(GUI_DIALOG_TITLE_PAD, GUI_DIALOG_TITLE_PAD) );
		}


		/*=============================================================================
		-- Sets the caption of the text in the middle.
		=============================================================================*/
		void DialogBox::SetCaption(String caption)
		{
			mCaption = caption;
			WeakPtr<Text> text = DynamicPtrCast<Text>(GetChild(IEI_DIALOGBOX_TEXT).lock());

			if (!text.expired())
			{
				text.lock()->SetText(caption);
				Font *font = text.lock()->GetFont();

				unsigned width = (unsigned)font->GetTextLengthPx(mCaption.GetStd());
				unsigned height = (unsigned)font->GetCharHeightPx();


				//clip any characters that don't fit inside from the text element
				while (width > GetWidth()-2*GUI_DIALOG_TITLE_PAD)
				{
					mCaption.Remove(mCaption.Size()-1,mCaption.Size());
					SetCaption(mCaption);
					width = (unsigned)font->GetTextLengthPx(mCaption.GetStd());
				}

				//properly center the text
				text.lock()->SetRelPos(Vector2D<int>(8,5));
			}
		}
	};
};