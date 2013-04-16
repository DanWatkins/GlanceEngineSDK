/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Rectangle.cpp
  Created: 3/4/2013
=================================================================================================*/
#include "../Render2D.h"


namespace ge
{
    /*=============================================================================
    -- Draws a rectangle using SFML's built in Shape class.
    =============================================================================*/
	void Rectangle::Draw()
	{
		sf::Shape rect = sf::Shape::Rectangle((float)mP1.x, (float)mP1.y, (float)mP2.x, (float)mP2.y,
												GetColor().Get(), 0.0, GetColor().Get());
		rect.EnableFill(mFilled);
		rect.EnableOutline(!mFilled);

		if (!mFilled)
			rect.SetOutlineWidth(GetThickness());
		
		GetWindow()->GetWindow()->Draw(rect);
	}
}