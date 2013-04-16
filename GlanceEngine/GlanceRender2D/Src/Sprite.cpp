/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Sprite.cpp
=================================================================================================*/
#include "Render2D.h"

namespace ge
{
    /*=============================================================================
    -- Constructor for Sprite.
    =============================================================================*/
    Sprite::Sprite()
    {
        mIsSheet            = false;
        mFrame              = 0;
        mColumns            = 1;
        mFrameWidth         = 0;
        mFrameHeight        = 0;
		mWindow				= NULL;
    }


	/*=============================================================================
    -- Calculates the clipping region to draw only part of a sprite sheet. If this
	   is not currently a sprite sheet, then the clipping region is set to the
	   entire image.
    =============================================================================*/
	void Sprite::_RecalculateClipping()
	{
		if (mIsSheet)
		{
			mClipping.Left = (mFrame % mColumns) * mFrameWidth;
			mClipping.Top = (mFrame / mColumns) * mFrameHeight;
			mClipping.Right = mClipping.Left + mFrameWidth;
			mClipping.Bottom = mClipping.Top + mFrameHeight;
		
            mSprite.SetSubRect(mClipping);
		}
		else
		{
			sf::IntRect defaultSubRect;
			defaultSubRect.Left = 0;
			defaultSubRect.Top = 0;
			defaultSubRect.Right = mSprite.GetImage()->GetWidth();
			defaultSubRect.Bottom = mSprite.GetImage()->GetHeight();

			mSprite.SetSubRect(defaultSubRect);
		}
	}


    /*=============================================================================
    -- Sets the render window pointer and the image the sprite uses.
    =============================================================================*/
    bool Sprite::Init(Window *window, sf::Image *image)
    {
        mWindow = window;

		if (image)
		{
			mSprite.SetImage(*image);
			_RecalculateClipping();
		}

		return (mSprite.GetImage() != NULL);
    }


    /*=============================================================================
    -- Loads the image from a specific file.
    =============================================================================*/
    bool Sprite::LoadImageFromFile(String filepath)
    {
        io::Standardize(filepath);

		if (!mImage.LoadFromFile(filepath.GetStd()))
            return false;

        mImage.SetSmooth(false);
        mSprite.SetImage(mImage);

        //SetFrameWidth(mImage.GetWidth());
        //SetFrameHeight(mImage.GetHeight());

        return true;
    }


    /*=============================================================================
    -- Draws the sprite with offset @x,@y using all of the current states.
    =============================================================================*/
    void Sprite::Draw(int x, int y)
    {
        float nx = mSprite.GetPosition().x + (float)x;
        float ny = mSprite.GetPosition().y + (float)y;
        
        mSprite.SetPosition(nx, ny);
        Draw();
        mSprite.SetPosition(nx - (float)x, ny - (float)y);
    }


    /*=============================================================================
    -- Slightly faster drawing method. It does not need to calculate offset
       positions and then revert.
    =============================================================================*/
    void Sprite::Draw()
	{
        if (mWindow != NULL)
            mWindow->GetWindow()->Draw(mSprite);
    }


    /*=============================================================================
    -- Sets the horizontal (x) scale factor to make the source image seem like it
	   is the specified width.
    =============================================================================*/
    void Sprite::SetWidth(unsigned width)
    {
        if (mSprite.GetImage())
            SetScaleX( DivideF((float)width, (float)mSprite.GetImage()->GetWidth()) );
    }


	/*=============================================================================
    -- Returns the width of the image to be displayed. It takes into account
	   scale factors. If this is a sheet, then just the frame to be displayed is
	   used instead of the entire image.
    =============================================================================*/
    unsigned Sprite::GetWidth()
    {
        if (mSprite.GetImage())
            return (unsigned)(GetScaleX()  *mSprite.GetImage()->GetWidth());

        return 0;
    }


	/*=============================================================================
    -- Sets the vertical (y) scale factor to make the source image seem like it
	   is the specified width.
    =============================================================================*/
    void Sprite::SetHeight(unsigned height)
    {
        if (mSprite.GetImage())
            SetScaleY( DivideF((float)height, (float)mSprite.GetImage()->GetHeight()) );
    }


	/*=============================================================================
    -- Returns the height of the image to be displayed. It takes into account
	   scale factors. If this is a sheet, then just the frame to be displayed is
	   used instead of the entire image.
    =============================================================================*/
    unsigned Sprite::GetHeight()
    {
        if (mSprite.GetImage())
            return (unsigned)(GetScaleY()  *mSprite.GetImage()->GetHeight());

        return 0;
    }
};