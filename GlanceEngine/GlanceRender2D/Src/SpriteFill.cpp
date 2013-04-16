/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: SpriteFill.cpp
=================================================================================================*/
#include "Render2D.h"

namespace ge
{
    /*=============================================================================
      Constructor for SpriteFill.
    =============================================================================*/
    SpriteFill::SpriteFill()
    {
        mTexture.SetIsSheet(true);
        mFillWidth = 256;
        mFillHeight = 256;
    }


    /*=============================================================================
      Loads the texture (Sprite) for use and sets the window to render to.
    =============================================================================*/
    bool SpriteFill::Init(String filepath, Window *window)
    {
        SetWindow(window);
        return mTexture.LoadImageFromFile(filepath);
    }


    /*=============================================================================
      Draws the texture inside of @mFillWidth and @mFillHeight.
    =============================================================================*/
    void SpriteFill::Draw()
    {
        unsigned w = GetTextureWidth();
        unsigned h = GetTextureHeight();
        int x = GetX();
        int y = GetY();
        int oldx = x;
        int oldy = y;
        int endx = x+mFillWidth;
        int endy = y+mFillHeight;

        while (y < endy)
        {
            //make sure the texture is not taller than the fill area
            if ( (endy-y) < (int)h )
				mTexture.SetFrameHeight( (unsigned)endy-y );

            mTexture.SetY(y);

            while (x < endx)
            {
                mTexture.SetX(x);

                //make sure the texture is not wider than the fill area
                if ( endx-x < (int)w )
                    mTexture.SetFrameWidth( (unsigned)(endx-x) );

                mTexture.Draw();

                x += w;
            }

            //reset the width of the texture
            if ( (endx-x) < (int)w )
                    mTexture.SetFrameWidth(w);

            x = oldx;
            y += h;
        }

        //reset the height and position of the texture
        if ( endy-y < (int)h )
            mTexture.SetFrameHeight(h);

        mTexture.SetPos(oldx, oldy);
    }
};