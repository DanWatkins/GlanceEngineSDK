/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: GroupBox.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for GroupBox.
		=============================================================================*/
		GroupBox::GroupBox()
		{
			mCaption = "";
			SetCanFocus(false);
		}


		/*=============================================================================
		-- Initializes the GroupBox after it has been added to the Root. Automatically
		   called from its corresponding Root::Create() method.
		=============================================================================*/
		bool GroupBox::_PostInit()
		{
			//load border sprite frame from memory
			mImgBorder.Init( GetWidth(), GetHeight(), GetRoot()->GetWindow() );
			((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("groupbox"))->PassImagesToSprite(&mImgBorder);

			//create text integrated element
			GetRoot()->CreateText(IEI_GROUPBOX_TEXT, GetId(), Vector2D<int>(), mCaption);
			WeakPtr<Text> text = DynamicPtrCast<Text>(GetChild(IEI_GROUPBOX_TEXT).lock());

			//properly space the text
			if (!text.expired())
			{
				unsigned textHeight = (unsigned)text.lock()->GetFont()->GetCharHeightPx();
				unsigned frameThickness = (unsigned)mImgBorder.GetCornerHeight();
				int verticalOffset = -(int)((textHeight/2)-(frameThickness/2))+text.lock()->GetFont()->GetVerticalOffset();

				text.lock()->SetRelPos( Vector2D<int>(GUI_GROUPBOX_CAPTION_OFFSET, verticalOffset)  );

				//set the break period in the border
				text.lock()->_UpdateAssets();
				mImgBorder.SetTopBarBreakPos( GUI_GROUPBOX_CAPTION_OFFSET-mImgBorder.GetCornerWidth()-GUI_GROUPBOX_CAPTION_PAD_LEFT,
												GUI_GROUPBOX_CAPTION_OFFSET-mImgBorder.GetCornerWidth()+text.lock()->GetWidth()+GUI_GROUPBOX_CAPTION_PAD_LEFT );
			}

			mImgBorder.SetShowCenter(false);

			return true;
		}


		/*=============================================================================
		-- Draws the GroupBox.
		=============================================================================*/
		void GroupBox::_Draw()
		{
			mImgBorder.Draw();
		}


		/*=============================================================================
		-- Updates the scalar metrics of any assets.
		=============================================================================*/
		void GroupBox::_UpdateAssets()
		{
			mImgBorder.SetPos(GetScreenPos());
			mImgBorder.SetSize( GetWidth(), GetHeight() );
			mImgBorder.ResetPos();
			mImgBorder.ResetSize();
		}
	};
};