/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: CheckBox.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for CheckBox.
		=============================================================================*/
		CheckBox::CheckBox()
		{
			_SetState(UNCHECKED);
			mPressed = false;
		}


		void CheckBox::SetChecked(bool checked)
		{
			if (checked)
				_SetState(CHECKED);
			else
				_SetState(UNCHECKED);
		}

		/*=============================================================================
		-- Initializes the CheckBox after it has been added to the Root. Automatically
		   called from its corresponding Root::Create() method.
		=============================================================================*/
		bool CheckBox::_PostInit()
		{
			//initialize
			mImgChecked.Init(GetRoot()->GetWindow(), ((Image*)GetRoot()->GetAssetManager()->GetAsset(GAID_CHECKED))->GetImage());
			mImgUnchecked.Init(GetRoot()->GetWindow(), ((Image*)GetRoot()->GetAssetManager()->GetAsset(GAID_UNCHECKED))->GetImage());
			mImgCheckedDisabled.Init(GetRoot()->GetWindow(), ((Image*)GetRoot()->GetAssetManager()->GetAsset(GAID_CHECKED_DISABLED))->GetImage());
			mImgUncheckedDisabled.Init(GetRoot()->GetWindow(), ((Image*)GetRoot()->GetAssetManager()->GetAsset(GAID_UNCHECKED_DISABLED))->GetImage());

			//take the width of mImgChecked
			_SetSize( (unsigned)mImgChecked.GetWidth(), (unsigned)mImgChecked.GetHeight());

			return true;
		}


		/*=============================================================================
		-- Updates the CheckBox.
		=============================================================================*/
		void CheckBox::_Update()
		{
			//if the box was pressed in a previous frame, and now the left mouse button has been released
			if (mPressed && !GetRoot()->GetWindow()->GetInput()->GetButtonState(GMB_Left))
			{
				//toggle the checked/unchecked state
				if (GetState() == CHECKED)
					_SetState(UNCHECKED);
				else
					_SetState(CHECKED);

				mPressed = false;
			}
		}


		/*=============================================================================
		-- Draws the CheckBox. Determines if it is disabled and what state it is in
		   to draw 1 of 4 possible images.
		=============================================================================*/
		void CheckBox::_Draw()
		{
			if (!GetDisabled())
			{
				//checked, enabled
				if (GetState() == CHECKED)
					mImgChecked.Draw();
				//unchecked, enabled
				else
					mImgUnchecked.Draw();
			}
			else
			{
				//checked, disabled
				if (GetState() == CHECKED)
					mImgCheckedDisabled.Draw();
				//unchecked, disabled
				else
					mImgUncheckedDisabled.Draw();
			}

			if (mTextAlignment == TextAlignment::AlignedRight)
				GetRoot()->GetDefaultFont()->Print(mCaption, GetScreenPos().x+GetWidth()+6, GetScreenPos().y);//TODO this could use a define
			else
			{
				int textLength = GetRoot()->GetDefaultFont()->GetTextLengthPx(mCaption);
				GetRoot()->GetDefaultFont()->Print(mCaption, GetScreenPos().x-6-textLength, GetScreenPos().y);
			}
		}


		/*=============================================================================
		-- Sends an event message to the CheckBox for it to process.
		=============================================================================*/
		void CheckBox::_SendMessage(Message message)
		{
			//set @mPressed true and wait until the mouse button is released
			//to determine the checked or unchecked state
			if (message == MBL_DOWN_OVER)
			{
				if (!GetDisabled())
					mPressed = true;				
			}
		}


		/*=============================================================================
		-- Updates the scalar metrics of every image.
		=============================================================================*/
		void CheckBox::_UpdateAssets()
		{
			mImgChecked.SetPos(GetScreenPos());
			mImgUnchecked.SetPos(GetScreenPos());
			mImgCheckedDisabled.SetPos(GetScreenPos());
			mImgUncheckedDisabled.SetPos(GetScreenPos());
		}
	};
};