/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Line.cpp
  Created: 3/5/2013
=================================================================================================*/
#include "../Render2D.h"


namespace ge
{
	/*=============================================================================
    -- Constructor for Line.
    =============================================================================*/
	Line::Line(Window *window)
	{
		SetWindow(window);
		mThickness = 1;
	}


    /*=============================================================================
    -- Draws a rectangle using SFML's built in Shape class.
    =============================================================================*/
	void Line::Draw()
	{
		//just use SFML's build in Line drawer since there is no good way to draw points to do it manually
		sf::Shape line = sf::Shape::Line(mP1.x, mP1.y, mP2.x, mP2.y, mThickness, GetColor().Get(), 0.0, GetColor().Get());
		GetWindow()->GetWindow()->Draw(line);

		/*int x = mP1.x;
		int y = mP1.y;
		int dx = std::abs(mP2.x-mP1.x);
		int dy = std::abs(mP2.y-mP1.y);
		int xInc1=0, yInc1=0, xInc2=0, yInc2=0;

		//determine the direction of the line
		if (mP2.x >= mP1.x)
		{
			xInc1 = 1;
			xInc2 = 1;
		}
		else
		{
			xInc1 = -1;
			xInc2 = -1;
		}

		if (mP2.y >= mP1.y)
		{
			yInc1 = 1;
			yInc2 = 1;
		}
		else
		{
			yInc1 = -1;
			yInc2 = -1;
		}

		//configure the drawing process base on the direction
		int num=0, den=0;
		int numAdd=0, numPixels=0;

		if (dx >= dy)
		{
			xInc1 = 0;
			yInc2 = 0;
			den = dx;
			num = dx/2;
			numAdd = dy;
			numPixels = dx;
		}
		else
		{
			xInc2 = 0;
			yInc1 = 0;
			den = dy;
			num = dy/2;
			numAdd = dx;
			numPixels = dy;
		}

		//draw all the points on the line
		for (int n=0; n<=numPixels; n++)
		{
			//TODO need DrawPixel(int,int) method in ge::Window
			//GetWindow()->DrawPixel(x, y);

			num += numAdd;
			if (num >= den)
			{
				num -= den;
				x += xInc1;
				y += yInc1;
			}

			x += xInc2;
			y += yInc2;
		}*/
	}
}