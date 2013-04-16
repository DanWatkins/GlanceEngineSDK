/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: SpriteFill.h
=================================================================================================*/
#include "Render2D.h"

#ifndef _ge_SPRITE_FILL_H
#define _ge_SPRITE_FILL_H

namespace ge
{
    /*=============================================================================
    -- Can tessellate a single sprite to fill an area.
    =============================================================================*/
    class SpriteFill
    {
    private:
        Sprite mTexture;						//texture used to tessellate in the fill area
        unsigned mFillWidth, mFillHeight;		//size of the area to tessellate the texture


    public:
        SpriteFill();

		//general
        bool Init(String filepath, Window *window);
        void Draw();
        
		//accessor and mutator
        void SetPos(int x, int y) { SetX(x); SetY(y); }
        void SetX(int x) { mTexture.SetX(x); }
        int GetX() { return mTexture.GetX(); }
        void SetY(int y) { mTexture.SetY(y); }
        int GetY() { return mTexture.GetY(); }

        void SetFillArea(unsigned width, unsigned height) { SetFillWidth(width); SetFillHeight(height); }
        void SetFillWidth(unsigned width) { mFillWidth = width; }
        unsigned GetFillWidth() { return mFillWidth; }
        void SetFillHeight(unsigned height) { mFillHeight = height; }
        unsigned GetFillHeight() { return mFillHeight; }

        void SetTextureWidth(unsigned width) { mTexture.SetWidth(width); }
        unsigned GetTextureWidth() { return mTexture.GetWidth(); }
        void SetTextureHeight(unsigned height) { mTexture.SetHeight(height); }
        unsigned GetTextureHeight() { return mTexture.GetHeight(); }

        void SetWindow(Window *window) { mTexture.SetWindow(window); }
		Window *GetWindow() { return mTexture.GetWindow(); }

    };

};

#endif

