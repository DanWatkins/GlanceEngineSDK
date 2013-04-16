/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Sprite.h
=================================================================================================*/
#include "Render2D.h"

#ifndef _ge_SPRITE_H
#define _ge_SPRITE_H

namespace ge
{
    /*=============================================================================
    -- Sprite class for drawing stuff to the screen.
    =============================================================================*/
    class Sprite
    {
    private:
        //rendering
        Window *mWindow;								//pointer to the window to render the sprite in
        sf::Sprite mSprite;                             //SFML sprite object
        sf::Image mImage;                               //SFML image object. Not used if the sprite shares an image
        bool mIsSheet;                                  //if true, the sprite acts like a tilesheet and parts of it can be rendered
		sf::IntRect mClipping;							//cached clipping region to draw only parts of a sprite sheet (recalculated when frame size is set)

        //animation and sheet managment
        int mFrame;                                     //current frame to draw if it is a tilesheet. 0 is the top left, then going left to right, top to bottom.
        int mColumns;                                   //number of columns (default = 1)
        int mFrameWidth;                                //width of each section in the sheet
        int mFrameHeight;                               //height of each section in the sheet

		void _RecalculateClipping();

    public:
        //constructor and destructor
        Sprite();
        ~Sprite() {}

        //general
        void SetWindow(Window *window) { mWindow = window; }
		Window *GetWindow() { return mWindow; }
        bool Init(Window *window, sf::Image *image=NULL);
        bool LoadImageFromFile(String filepath);
        void Draw(int x, int y);
        void Draw();

        //accessors and mutators
        sf::Sprite *GetSprite() { return &mSprite; }
        void SetImage(sf::Image &image) { mSprite.SetImage(image); _RecalculateClipping(); }
        sf::Image *GetImage() { return &mImage; }
		void SetIsSheet(bool isSheet) { mIsSheet = isSheet; _RecalculateClipping(); }
        bool GetIsSheet() { return mIsSheet; }

		void SetFrame(int frame) { mFrame = frame; _RecalculateClipping(); }
        int GetFrame() { return mFrame; }
		void SetColumns(int columns) { mColumns = columns; _RecalculateClipping(); }
        int GetColumns() { return mColumns; }
		int GetRows() { return DivideI(GetHeight(), GetFrameHeight()); }
        void SetFrameDimensions(unsigned width, unsigned height) { SetFrameWidth(width); SetFrameHeight(height); }
        void SetFrameWidth(unsigned width) { mFrameWidth = width; _RecalculateClipping(); }
        unsigned GetFrameWidth() { return mFrameWidth; }
        void SetFrameHeight(unsigned height) { mFrameHeight = height; _RecalculateClipping();}
        unsigned GetFrameHeight() { return mFrameHeight; }

        int GetX() { return (int)mSprite.GetPosition().x; }
        void SetX(int x) { mSprite.SetX((float)x); }
        int GetY() { return (int)mSprite.GetPosition().y; }
        void SetY(int y) { mSprite.SetY((float)y); }
        void SetPos(int x, int y) { SetX(x); SetY(y); }
		void SetPos(Vector2D<int> pos) { SetPos( (int)pos.x, (int)pos.y); }

        void SetSize(unsigned width, unsigned height) { SetWidth(width); SetHeight(height); }
        void SetWidth(unsigned width);
        unsigned GetWidth();
        void SetHeight(unsigned height);
        unsigned GetHeight();

        void SetScaleX(float sx) { mSprite.SetScaleX(sx); }
        float GetScaleX() { return mSprite.GetScale().x; }
        void SetScaleY(float sy) { mSprite.SetScaleY(sy); }
        float GetScaleY() { return mSprite.GetScale().y; }

		void SetColor(Color color) { mSprite.SetColor(color.Get()); }
		Color GetColor() { return Color(mSprite.GetColor()); }
    };

};


#endif

