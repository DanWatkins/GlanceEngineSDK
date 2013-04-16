/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Rectangle.h
  Created: 3/4/2013
=================================================================================================*/
#include "../Render2D.h"

#ifndef _ge_RECTANGLE_H
#define _ge_RECTANGLE_H

namespace ge
{
    /*=============================================================================
    -- Rectangle class for drawing rectangles between two points.
    =============================================================================*/
	class Rectangle : public Shape
	{
	private:
		Vector2D<int> mP1, mP2;	//two points to draw the rectangle between
		bool mFilled;			//true if it sould have the center filled in
		int mThickness;			//thickness in pixels of the edges

	public:
		Rectangle() { Rectangle(NULL); }
		Rectangle(Window *window) { SetWindow(window); mThickness = 1; }

		void Draw();

		void SetP1(Vector2D<int> p1) { mP1 = p1; }
		void SetP1(int x, int y) { SetP1(Vector2D<int>(x,y)); }
		void SetP2(Vector2D<int> p2) { mP2 = p2; }
		void SetP2(int x, int y) { SetP2(Vector2D<int>(x,y)); }
		void SetPoints(Vector2D<int> p1, Vector2D<int> p2) { SetP1(p1); SetP2(p2); }
		void SetPoints(int x1, int y1, int x2, int y2) { SetP1(x1,y1); SetP2(x2,y2); }

		void SetFilled(bool filled) { mFilled = filled; }
		bool GetFilled() { return mFilled; }

		void SetThickness(int thickness) { mThickness = thickness; }
		int GetThickness() { return mThickness; }
	};
}

#endif

