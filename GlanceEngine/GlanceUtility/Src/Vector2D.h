/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Vector2D.h
=================================================================================================*/

#include "Utility.h"

#ifndef _ge_VECTOR2D_H
#define _ge_VECTOR2D_H

namespace ge
{
    /*=============================================================================
    -- 2D Vector class for storing 2D coordinates.
    =============================================================================*/
	template <class T>
    class Vector2D
    {
    public:
        T x, y;

		/*=============================================================================
		-- Default constructor for Vector2D.
		=============================================================================*/
        Vector2D()
		{
			x=(T)0.0;
			y=(T)0.0;
		
		}

		/*=============================================================================
		-- Overloaded constructor to specify the x,y pos.
		=============================================================================*/
        Vector2D(T x, T y)
		{
			this->x = x;
			this->y = y;
		}


		/*=============================================================================
		-- Templated copy constructor so two vectors of different types can be
		   interchanged. For example: Vector2D<int> v1; Vector2D<double> v2(v1);
		   That would be legal because of this copy constructor.
		=============================================================================*/
		template <class C>
		Vector2D(const Vector2D<C>& vect)
		{
			x = (T)vect.x;
			y = (T)vect.y;
		}


		/*=============================================================================
		-- Sets the vector to the specified values.
		=============================================================================*/
		void Set(T x, T y)
		{
			this->x = x;
			this->y = y;
		}


		/*=============================================================================
		-- Adds the values to the vector.
		=============================================================================*/
		void Add(T x, T y)
		{
			this->x += x;
			this->y += y;
		}


		/*=============================================================================
		-- Calculates the distance between @this and @vect.
		=============================================================================*/
		double Distance(Vector2D<T> vect)
		{
			return std::sqrt( (vect.x-x)*(vect.x-x) + (vect.y-y)*(vect.y-y) );
		}


		/*=============================================================================
		-- Calculates the dot product of @this and @vect.
		=============================================================================*/
		double DotProduct(Vector2D<T> vect)
		{
			return (vect.x*x) + (vect.y*y);
		}


		/*=============================================================================
		-- Calculates the angle between @this and @vect.
		=============================================================================*/
		double Angle(Vector2D<T> vect)
		{
			return std::atan2( (double)(vect.y-y), (double)(vect.x-x));  //TODO the cast may be too expensive
		}


		/*=============================================================================
		-- Calculates the normal of @this on the left or right side based on @leftSide.
		=============================================================================*/
		Vector2D Normal(Vector2D<T> vect, bool leftSide)
		{
			Vector2D normal;

			if (leftSide)
			{
				normal.x = -(vect.y-y);
				normal.y = (vect.x-x);
			}
			else
			{
				normal.x = (vect.y-y);
				normal.y = -(vect.x-x);
			}

			return normal;
		}


		/*=============================================================================
		-- Normalizes @this.
		=============================================================================*/
		void Normalize()
		{
			double length = std::sqrt(x*x + y*y);
			x /= length;
			y /= length;
		}


		/*=============================================================================
		-- Calculates a normalized velocity value from @this to @vect.
		=============================================================================*/
		Vector2D Velocity(Vector2D<T> vect)
		{
			Vector2D velocity;
			double angle = Angle(vect);

			if (angle < 0.0)
				angle += TWO_PI;

			velocity.x = (T)std::cos(angle*PI_OVER_180);
			velocity.y = (T)std::sin(angle*PI_OVER_180);

			return velocity;
		}


		/*=============================================================================
		-- Assignment operators
		=============================================================================*/
		void operator += (const Vector2D<T>& vect)
		{
			x += vect.x;
			y += vect.y;
		}

		void operator -= (const Vector2D<T>& vect)
		{
			x -= vect.x;
			y -= vect.y;
		}

		void operator *= (const Vector2D<T>& vect)
		{
			x *= vect.x;
			y *= vect.y;
		}

		void operator /= (const Vector2D<T>& vect)
		{
			x /= vect.x;
			y /= vect.y;
		}


		/*=============================================================================
		-- Expressional operators.
		=============================================================================*/
		Vector2D<T> operator + (const Vector2D<T>& vect)
		{
			return Vector2D<T>(x+vect.x, y+vect.y);
		}

		Vector2D<T> operator + (T n)
		{
			return Vector2D<T>(x+n, y+n);
		}

		Vector2D<T> operator - (const Vector2D<T>& vect)
		{
			return Vector2D<T>(x-vect.x, y-vect.y);
		}

		Vector2D<T> operator - (T n)
		{
			return Vector2D<T>(x-n, y-n);
		}
		
		Vector2D<T> operator  *(const Vector2D<T>& vect)
		{
			return Vector2D(x*vect.x, y*vect.y);
		}

		Vector2D<T> operator  *(T n)
		{
			return Vector2D<T>(x*n, y*n);
		}

		Vector2D<T> operator / (const Vector2D<T>& vect)
		{
			return Vector2D<T>(x/vect.x, y/vect.y);
		}

		Vector2D<T> operator / (T n)
		{
			return Vector2D<T>(x/n, y/n);
		}

		bool operator == (const Vector2D<T>& vect)
		{
			return (vect.x == x && vect.y == y);
		}

		bool operator != (const Vector2D<T>& vect)
		{
			return (vect.x != x && vect.y != y);
    	}
    };
};

#endif

