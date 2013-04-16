/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Font.h
=================================================================================================*/
#include "Render2D.h"

#ifndef _ge_FONT_H
#define _ge_FONT_H

namespace ge
{
    /*=============================================================================
    -- Font class for rendering text to the screen.
    =============================================================================*/
    class Font
    {
    private:
		Sprite mTexture;		//sprite containing all the pre-rendered characters (ASCII)
        int mWidths[256];		//proportional widths for each character
		int mVerticalOffset;	//TODO I don't think this is used


    public:
        Font();
        ~Font() {}

		//general
		bool LoadFontFromFile(String fontname);
        void Print(String text, int x, int y, Color color=gWhite);

		//accessor and mutator
		int GetTextLengthPx(String text);
		int GetCharHeightPx() { return mTexture.GetFrameHeight(); }
		void SetWindow(Window *window) { mTexture.SetWindow(window); }
		int GetVerticalOffset() { return mVerticalOffset; }
    };
    
};

#endif

