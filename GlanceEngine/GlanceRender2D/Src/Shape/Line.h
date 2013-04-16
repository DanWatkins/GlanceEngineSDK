/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Line.h
  Created: 3/5/2013
=================================================================================================*/
#include "../Render2D.h"

#ifndef _ge_LINE_H
#define _ge_LINE_H

namespace ge
{
    /*=============================================================================
    -- Line class for drawing lines between two points.
    =============================================================================*/
	class Line : public Shape
	{
	private:
		Vector2D<int> mP1, mP2;	//two points to draw the rectangle between
		int mThickness;			//thickness in pixels of the edges

	public:
		Line(Window *window);

		void Draw();

		void SetP1(Vector2D<int> p1) { mP1 = p1; }
		void SetP1(int x, int y) { SetP1(Vector2D<int>(x,y)); }
		void SetP2(Vector2D<int> p2) { mP2 = p2; }
		void SetP2(int x, int y) { SetP2(Vector2D<int>(x,y)); }
		void SetPoints(Vector2D<int> p1, Vector2D<int> p2) { SetP1(p1); SetP2(p2); }
		void SetPoints(int x1, int y1, int x2, int y2) { SetP1(x1,y1); SetP2(x2,y2); }

		void SetThickness(int thickness) { mThickness = thickness; }
		int GetThickness() { return mThickness; }
	};
}

#endif

