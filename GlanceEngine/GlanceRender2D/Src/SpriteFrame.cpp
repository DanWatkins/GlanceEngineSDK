/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: SpriteFrame.cpp
=================================================================================================*/
#include "Render2D.h"

namespace ge
{
    /*=============================================================================
    -- Constructor for Sprite.
    =============================================================================*/
    SpriteFrame::SpriteFrame()
    {
        x					= 0;
        y					= 0;
        mWidth				= 32;
        mHeight				= 32;
        mCornerWidth		= 3;
        mCornerHeight		= 3;
        mShowCenter			= true;
		mWindow				= NULL;
		mTopBarBreakStart	= 0;
		mTopBarBreakEnd		= 0;
    }
    

	/*=============================================================================
    -- Initializes the sprite frame but does not load images.
    =============================================================================*/
	bool SpriteFrame::Init(unsigned width, unsigned height, Window *window, bool showCenter)
	{
		//set parameters
        mWidth = width;
        mHeight = height;
        mShowCenter = showCenter;
		mWindow = window;

		return true;
	}

    /*=============================================================================
    -- Initializes the sprite and loads images.
    =============================================================================*/
    bool SpriteFrame::Init(unsigned width, unsigned height, String directory, Window *window, bool showCenter)
    {
		Init(width, height, window, showCenter);
		LoadImagesFromDirectory(directory);
		return true; //TODO lazy return lol
    }


	bool SpriteFrame::LoadImagesFromDirectory(String directory)
	{
		mCenter.LoadImageFromFile(directory.GetStd()+"center.png");
		mTop.LoadImageFromFile(directory.GetStd()+"top.png");
		mTopLeft.LoadImageFromFile(directory.GetStd()+"top_left.png");
		mTopRight.LoadImageFromFile(directory.GetStd()+"top_right.png");
		mLeft.LoadImageFromFile(directory.GetStd()+"left.png");
		mRight.LoadImageFromFile(directory.GetStd()+"right.png");
		mBottom.LoadImageFromFile(directory.GetStd()+"bottom.png");
		mBottomLeft.LoadImageFromFile(directory.GetStd()+"bottom_left.png");
		mBottomRight.LoadImageFromFile(directory.GetStd()+"bottom_right.png");

		mCenter.SetWindow(mWindow);
        mLeft.SetWindow(mWindow);
        mTop.SetWindow(mWindow);
        mRight.SetWindow(mWindow);
        mBottom.SetWindow(mWindow);
        mTopLeft.SetWindow(mWindow);
        mTopRight.SetWindow(mWindow);
        mBottomLeft.SetWindow(mWindow);
        mBottomRight.SetWindow(mWindow);

		//make sure everything is lined up
        ResetSize();
        ResetPos();

		return true;
	}

	/*=============================================================================
    -- Loads the images for each sprite from pre-loaded images.
    =============================================================================*/
	bool SpriteFrame::LoadImagesFromMemory(sf::Image *center, sf::Image *left, sf::Image *top, sf::Image *right,
											sf::Image *bottom, sf::Image *topLeft, sf::Image *topRight,
											sf::Image *bottomLeft, sf::Image *bottomRight)
	{
		//set the images
		mCenter.SetImage(*center);
		mLeft.SetImage(*left);
		mTop.SetImage(*top);
		mRight.SetImage(*right);
		mBottom.SetImage(*bottom);
		mTopLeft.SetImage(*topLeft);
		mTopRight.SetImage(*topRight);
		mBottomLeft.SetImage(*bottomLeft);
		mBottomRight.SetImage(*bottomRight);

		//set the render window for every sub-sprite
		mCenter.SetWindow(mWindow);
        mLeft.SetWindow(mWindow);
        mTop.SetWindow(mWindow);
        mRight.SetWindow(mWindow);
        mBottom.SetWindow(mWindow);
        mTopLeft.SetWindow(mWindow);
        mTopRight.SetWindow(mWindow);
        mBottomLeft.SetWindow(mWindow);
        mBottomRight.SetWindow(mWindow);

		//make sure everything is lined up
        ResetSize();
        ResetPos();

		return true;
	}


    /*=============================================================================
    -- Draws every sub-sprite the way they are.
    =============================================================================*/
    void SpriteFrame::Draw()
    {
        if (mShowCenter)
            mCenter.Draw();

        mLeft.Draw();
        mRight.Draw();
        mBottom.Draw();

        mTopLeft.Draw();
        mTopRight.Draw();
        mBottomLeft.Draw();
        mBottomRight.Draw();

		//draw top bar with breaks
		int oldWidth = mTop.GetWidth();
		mTop.SetWidth(mTopBarBreakStart);
		mTop.Draw();
		mTop.SetWidth(oldWidth-mTopBarBreakEnd);
		mTop.SetX(mTop.GetX()+mTopBarBreakEnd);
		mTop.Draw();

		//revert to the old parameters
		mTop.SetX(mTop.GetX()-mTopBarBreakEnd);
		mTop.SetWidth(oldWidth);
    }


    /*=============================================================================
    -- Resets the position of very sub-sprite relative to the coordinates of the
       overall sprite. Should be called every time the overall sprite gets new
       x,y coordinates or there is a size change.
    =============================================================================*/
    void SpriteFrame::ResetPos()
    {
        mCenter.SetPos(x+mCornerWidth, y+mCornerHeight);

        mLeft.SetPos(x, y+mCornerHeight);
        mTop.SetPos(x+mCornerWidth, y);
        mRight.SetPos(x+mWidth-mCornerWidth, y+mCornerHeight);
        mBottom.SetPos(x+mCornerWidth, y+mHeight-mCornerHeight);

        mTopLeft.SetPos(x, y);
        mTopRight.SetPos(x+mWidth-mCornerWidth, y);
        mBottomLeft.SetPos(x, y+mHeight-mCornerHeight);
        mBottomRight.SetPos(x+mWidth-mCornerWidth, y+mHeight-mCornerHeight);
    }


    /*=============================================================================
    -- Resets the size of every sub-sprite. Should be called every time there is
       a size change in the overall sprite or new corner sizes are assigned.
    =============================================================================*/
    void SpriteFrame::ResetSize()
    {
        //make sure the overall sprite is as big as it should be
        if (mWidth < (mCornerWidth*2))
            mWidth = (mCornerWidth*2);
        if (mHeight < (mCornerHeight*2))
            mHeight = (mCornerHeight*2);

        //set each sub-sprites size relative to the overall sprite
        mCenter.SetSize(mWidth-(mCornerWidth*2), mHeight-(mCornerHeight*2));

        mTopLeft.SetSize(mCornerWidth, mCornerHeight);
        mTopRight.SetSize(mCornerWidth, mCornerHeight);
        mBottomLeft.SetSize(mCornerWidth, mCornerHeight);
        mBottomRight.SetSize(mCornerWidth, mCornerHeight);

        mLeft.SetSize(mCornerWidth, mHeight-(mCornerHeight*2));
        mTop.SetSize(mWidth-(mCornerWidth*2), mCornerHeight);
        mRight.SetSize(mCornerWidth, mHeight-(mCornerHeight*2));
        mBottom.SetSize(mWidth-(mCornerWidth*2), mCornerHeight);
    }


    /*=============================================================================
    -- Various accessor and mutator methods.
    =============================================================================*/
    void SpriteFrame::SetPos(int x, int y)
    {
        this->x = x;
        this->y = y;
        ResetPos();
    }

    void SpriteFrame::SetX(int x)
    {
        this->x = x;
        ResetPos();
    }

    void SpriteFrame::SetY(int y)
    {
        this->y = y;
        ResetPos();
    }

    //overall size
    void SpriteFrame::SetSize(unsigned width, unsigned height)
    {
        mWidth = width;
        mHeight = height;
        ResetSize();
        ResetPos(); 
    }

    void SpriteFrame::SetWidth(unsigned width)
    {
        mWidth = width;
        ResetSize();
        ResetPos(); 
    }

    void SpriteFrame::SetHeight(unsigned height)
    {
        mHeight = height;
        ResetSize();
        ResetPos(); 
    }

    //corner size
    void SpriteFrame::SetCornerSize(unsigned width, unsigned height)
    {
        mCornerWidth = width;
        mCornerHeight = height;
        ResetSize();
        ResetPos(); 
    }

    void SpriteFrame::SetCornerWidth(unsigned width)
    {
        mCornerWidth = width;
        ResetSize();
        ResetPos(); 
    }

    void SpriteFrame::SetCornerHeight(unsigned height)
    {
        mCornerHeight = height;
        ResetSize();
        ResetPos(); 
    }
};