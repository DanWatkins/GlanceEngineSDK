/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Font.cpp
=================================================================================================*/
#include "Render2D.h"

namespace ge
{
	/*=============================================================================
	-- Constructor for Font.
	=============================================================================*/
	Font::Font()
	{
		mVerticalOffset = 0;
		std::memset(&mWidths, 0, sizeof(mWidths));
	}
	

	/*=============================================================================
	-- Loads the font width data, texture dimensions, and the texture itself.
	=============================================================================*/
	bool Font::LoadFontFromFile(String fontname)
	{
		//prefix the font directory
		fontname = ToString(DIR_FONTS)+fontname;

		unsigned char buffer[512];

		std::ifstream file;
		file.open( (fontname+".dat").GetChar(), std::ios::binary);
		if (!file)
			return false;

		//read in 512 bytes
		file.read( (char*)(&buffer), 512 );
		if (file.bad())
			return false;

		//convert from raw data to width data
		for (int n=0; n<256; n++)
			mWidths[n] = (int)buffer[n*2];
				
		//load the font ini file
		HMLFile iniFile;
		iniFile.Open( fontname+".ini" );
		String imagePath = DIR_FONTS;

		if (iniFile.IsOpen())
		{
			imagePath.PushBack(iniFile.SeekGetToken());
			mVerticalOffset = ToInt(iniFile.SeekGetToken());
		}

		//load the font image
		if (!mTexture.LoadImageFromFile(imagePath))
			return false;

		mTexture.SetFrameDimensions( mTexture.GetWidth()/16, mTexture.GetHeight()/16 );
		mTexture.SetIsSheet(true);
		mTexture.SetColumns(16);

		return true;
	}


	/*=============================================================================
	-- Draws @text to the window at @x,@y in @color.
	=============================================================================*/
	void Font::Print(String text, int x, int y, Color color)
	{
		mTexture.SetColor(color);

		float fx = (float)x;
		float fy = (float)y;

		//subtract the white space befre the first character
		if (text.Size() > 0)
		{
			
			int firstChar = (int)*text[0].GetChar();
			fx -= (int)(mTexture.GetFrameWidth() - mWidths[firstChar]) / 2.0f;

			for (unsigned n=0; n<text.Size(); n++)
			{

				int frame = (int)*text[n].GetChar();
				mTexture.SetFrame(frame);
				mTexture.Draw(RoundDown(fx), RoundDown(fy));

				fx += (float)mWidths[frame];
			}
		}
	}


	/*=============================================================================
	-- Returns the length in pixels of @text.
	=============================================================================*/
	int Font::GetTextLengthPx(String text)
	{
		float fx = 0.0f;

		//subtract the white space befre the first character
		if (text.Size() > 0)
		{
			//int firstChar = (int)*text[0].GetChar();
			//fx -= (int)(mTexture.GetFrameWidth() - mWidths[firstChar]) / 2.0;

			for (unsigned n=0; n<text.Size(); n++)
			{
				int frame = (int)*text[n].GetChar();
				fx += (float)mWidths[frame];
			}
		}

		return RoundUp(fx);
	}
};