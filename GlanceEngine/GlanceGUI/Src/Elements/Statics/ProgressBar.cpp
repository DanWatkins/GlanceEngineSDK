/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ProgressBar.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for ProgressBar.
		=============================================================================*/
		ProgressBar::ProgressBar()
		{
			mPercent = 0.0;
			SetCanFocus(false);
		}


		/*=============================================================================
		-- Initializes the ProgressBar after it has been added to the Root. Automatically
		   called from its corresponding Root::Create() method.
		=============================================================================*/
		bool ProgressBar::_PostInit()
		{
			//load the images
			mImgBar.Init(GetWindow(), ((Image*)GetRoot()->GetAssetManager()->GetAsset(GAID_BAR))->GetImage());
			mImgBackground.Init( GetWidth(), GetHeight(), GetWindow() );
			((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("progressbar_background"))->PassImagesToSprite(&mImgBackground);

			//create the text element and link @mText with it
			if (!GetRoot()->CreateText(IEI_PROGRESSBAR_TEXT, GetId(), Vector2D<int>(), "").expired())
				mText = DynamicPtrCast<Text>(GetChild(IEI_PROGRESSBAR_TEXT).lock());

			return true;
		}


		/*=============================================================================
		-- Draws the background and then the bar on top. Text is a child and is
		   automatically drawn after this.
		=============================================================================*/
		void ProgressBar::_Draw()
		{
			mImgBackground.Draw();
			mImgBar.Draw();
		}


		/*=============================================================================
		-- Updates the scalar metrics of any assets.
		=============================================================================*/
		void ProgressBar::_UpdateAssets()
		{
			//set positions
			mImgBackground.SetPos(GetScreenPos());
			mImgBackground.ResetPos();

			Vector2D<int> newPos = GetScreenPos();
			newPos.x += GUI_PROGRESSBAR_PAD;
			newPos.y += GUI_PROGRESSBAR_PAD;
			mImgBar.SetPos( newPos );

			//stretch the bar according to the percentage
			mImgBar.SetHeight(GetHeight() - GUI_PROGRESSBAR_PAD*2);
			unsigned ampleWidth = GetWidth() - GUI_PROGRESSBAR_PAD*2;
			mImgBar.SetWidth( (unsigned) (ampleWidth*mPercent/100.0) );

		}


		/*=============================================================================
		-- Sets the text and properly centers it.
		=============================================================================*/
		void ProgressBar::SetText(String caption)
		{
			if (!mText.expired())
			{
				//truncate @caption until it fits
				while (mText.lock()->GetFont()->GetTextLengthPx(caption) > (int)GetWidth()-GUI_PROGRESSBAR_TEXT_PAD*2)
					caption.RemoveLast(1);

				mText.lock()->SetText(caption);

				//reset the text rel pos
				unsigned height = mText.lock()->GetFont()->GetCharHeightPx();
				unsigned width = mText.lock()->GetFont()->GetTextLengthPx(caption);
				Vector2D<int> offset;
				offset.x = (GetWidth()-width)/2;
				offset.y = (GetHeight()-height)/2;
				mText.lock()->SetRelPos(offset);
			}
		}


		/*=============================================================================
		-- Sets the percent and ensures that it is withing the range 0.0 to 100.0.
		=============================================================================*/
		void ProgressBar::SetPercent(double percent, int precision)
		{
			//bounds check
			if (percent < 0.0)
				mPercent = 0.0;
			else if (percent > 100.0)
				mPercent = 100.0;
			else
				mPercent = percent;


			//set the text if it isn't already something besides a percent
			if (!mText.expired())
			{
				//check if it is numeric by skipping the last character which should be a percent symbol
				if ( mText.lock()->GetText().IsNumeric(0, mText.lock()->GetText().Size()-1) )
					SetText( ToString(percent, precision)+"%" );
			}

			_UpdateAssets();
		}
	}
}