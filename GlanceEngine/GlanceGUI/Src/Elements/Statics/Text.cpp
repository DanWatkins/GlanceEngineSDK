/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Text.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for Text.
		=============================================================================*/
		Text::Text()
		{
			mFont = NULL;
			mTextAlignment = TextAlignment::AlignedLeft;
			SetCanFocus(false);
		}


		/*=============================================================================
		-- Draws the text.
		=============================================================================*/
		void Text::_Draw()
		{
			if (mFont)
			{
				if (mTextAlignment == TextAlignment::AlignedLeft)
					mFont->Print(mText, GetScreenPos().x, GetScreenPos().y, gWhite);
				else if (mTextAlignment == TextAlignment::AlignedRight)
				{
					int textLengthPx = mFont->GetTextLengthPx(mText);
					int textPosX = GetWidth()-textLengthPx+GetScreenPos().x;
					mFont->Print(mText, textPosX, GetScreenPos().y, gWhite);
				}
			}
		}


		/*=============================================================================
		-- Updates the scalar metrics of any assets.
		=============================================================================*/
		void Text::_UpdateAssets()
		{
			SetSize(GetFont()->GetTextLengthPx(mText), GetHeight());
		}

	};
};