/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Rect.h
=================================================================================================*/

#include "Utility.h"

#ifndef _ge_RECT_H
#define _ge_RECT_H

namespace ge
{
    /*=============================================================================
    -- Rectangle class which can test for intersection with other retangles.
    =============================================================================*/
	template <class T>
    class Rect
    {
    private:
        T x, y;                                //position of the upper left corner
        T mWidth, mHeight;                     //dimensions of the rectangle

    public:
		/*=============================================================================
		-- Constructor for Rect.
		=============================================================================*/
        Rect()
        {
            x=(T)0.0; y=(T)0.0; mWidth=(T)0.0; mHeight=(T)0.0;
        }

		Rect(T x, T y, T width, T height)
		{
			this->x = x;
			this->y = y;
			mWidth = width;
			mHeight = height;
		}

        ~Rect() {}

		/*=============================================================================
		-- Returns true if the @this is intersecting @rect.
		=============================================================================*/
		bool Intersect(Rect rect)
		{
			//check for situations that make it impossible for an intersection to occur
			if (GetBottom() < rect.GetTop())
				return false;
			if (GetTop() > rect.GetBottom())
				return false;
			if (GetRight() < rect.GetLeft())
				return false;
			if (GetLeft() > rect.GetRight())
				return false;

			return true;
		}


		/*=============================================================================
		-- Returns true if @point is inside @this.
		=============================================================================*/
		bool Intersect(Vector2D<T> point)
		{
			if (point.x > GetLeft() && point.x < GetRight())
			{
				if (point.y > GetTop() && point.y < GetBottom())
				{
					return true;
				}
			}

			return false;
		}


		/*=============================================================================
		-- Sets the position and size of the rect.
		=============================================================================*/
		void Set(T x, T y, T width, T height)
		{
			SetPos(x, y);
			SetSize(width, height);
		}


		/*=============================================================================
		-- Sets the position of the rect.
		=============================================================================*/
		void SetPos(T x, T y)
		{
			this->x = x;
			this->y = y;
		}


		/*=============================================================================
		-- Sets the size (dimensions) of the rectangle.
		=============================================================================*/
		void SetSize(T width, T height)
		{
			mWidth = width;
			mHeight = height;
		}


		//accessor and mutator
		Vector2D<T> GetPos() { return Vector2D<T>(x, y); }
        T GetLeft() { return x; }
        T GetTop() { return y; }
        T GetRight() { return x+mWidth; }
        T GetBottom() { return y+mHeight; }

        void SetWidth(T width) { mWidth = width; }
        void SetHeight(T height) { mHeight = height; }
        T GetWidth() { return mWidth; }
        T GetHeight() { return mHeight; }
	};

};

#endif

