/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: SpriteFrame.h
=================================================================================================*/
#include "Render2D.h"

#ifndef _ge_SPRITE_FRAME_H
#define _ge_SPRITE_FRAME_H

namespace ge
{
    /*=============================================================================
    -- Composed of 9 regular sprites to allow for scalable interface elements.
    =============================================================================*/
    class SpriteFrame
    {
    private:
		Window *mWindow;											//pointer to the windows this is drawn to

        //9 actual sprites
        Sprite mCenter;												//center
        Sprite mLeft, mTop, mRight, mBottom;						//edges
        Sprite mTopLeft, mTopRight, mBottomLeft, mBottomRight;		//corner

        //parameters
        int x, y;													//position of sprite (top-left corner)
        unsigned mWidth, mHeight;									//overall dimensions of the sprite
        unsigned mCornerWidth, mCornerHeight;						//size of each corner
        bool mShowCenter;                                           //false if the center should not be drawn
		int mTopBarBreakStart, mTopBarBreakEnd;						//positions to have a gap in the top bar image (used for text headers)


    public:
        SpriteFrame();
        ~SpriteFrame() {}

		//general
		bool Init(unsigned width, unsigned height, Window *window, bool showCenter=true);
        bool Init(unsigned width, unsigned height, String directory, Window *window, bool showCenter=true);
		bool LoadImagesFromDirectory(String directory);
		bool LoadImagesFromMemory(sf::Image *center, sf::Image *left, sf::Image *top, sf::Image *right, sf::Image *bottom,
								sf::Image *topLeft, sf::Image *topRight, sf::Image *bottomLeft, sf::Image *bottomRight);
        void Draw();
        void Reset() { ResetSize(); ResetPos(); }
        void ResetPos();
        void ResetSize();


		//accessor and mutator
        void SetPos(int x, int y);
		void SetPos(Vector2D<int> pos) { SetPos( (int)pos.x, (int)pos.y ); }
        void SetX(int x);
        void SetY(int y);

        void SetSize(unsigned width, unsigned height);
        void SetWidth(unsigned width);
        void SetHeight(unsigned height);

        void SetCornerSize(unsigned width, unsigned height);
        void SetCornerWidth(unsigned width);
        void SetCornerHeight(unsigned height);

        void SetShowCenter(bool showCenter) { mShowCenter = showCenter; }

        unsigned GetWidth() { return mWidth; }
        unsigned GetHeight() { return mHeight; }
        unsigned GetCornerWidth() { return mCornerWidth; }
        unsigned GetCornerHeight() { return mCornerHeight; }

		void SetTopBarBreakPos(unsigned start, unsigned end) { mTopBarBreakStart = start; mTopBarBreakEnd = end; }
    };

};

#endif

