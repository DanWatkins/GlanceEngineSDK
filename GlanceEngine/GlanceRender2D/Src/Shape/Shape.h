/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Shape.h
  Created: 3/4/2013
=================================================================================================*/
#include "../Render2D.h"

#ifndef _ge_SHAPE_H
#define _ge_SHAPE_H

namespace ge
{
    /*=============================================================================
    -- Rectangle class for drawing rectangles between two points.
    =============================================================================*/
	class Shape
	{
	private:
		Window *mWindow;
		Color mColor;

	public:
		Shape() { mWindow = NULL; }

		virtual void Draw() = 0;

		void SetWindow(Window *window) { mWindow = window; }
		Window *GetWindow() { return mWindow; }

		void SetColor(Color color) { mColor = color; }
		Color GetColor() { return mColor; }
	};
}

#endif

