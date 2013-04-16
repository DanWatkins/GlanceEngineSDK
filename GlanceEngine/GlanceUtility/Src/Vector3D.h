/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Vector3D.h
=================================================================================================*/

#include "Utility.h"

#ifndef _ge_VECTOR3D_H
#define _ge_VECTOR3D_H

namespace ge
{
    /*=============================================================================
    -- 3D Vector class for storing 3D coordinates.
    =============================================================================*/
	template <class T>
    class Vector3D
    {
    public:
        T x, y, z;

		/*=============================================================================
		-- Default constructor for Vector3D.
		=============================================================================*/
        Vector3D()
		{
			x = (T)0.0;
			y = (T)0.0;
			z = (T)0.0;
		}

		/*=============================================================================
		-- Overloaded constructor to specify the x,y pos.
		=============================================================================*/
        Vector3D(T x, T y, T z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}


		/*=============================================================================
		-- Templated copy constructor so two vectors of different types can be
		   interchanged. For example: Vector3D<int> v1; Vector3D<double> v2(v1);
		   That would be legal because of this copy constructor.
		=============================================================================*/
		template <class C>
		Vector3D(const Vector3D<C>& vect)
		{
			x = (T)vect.x;
			y = (T)vect.y;
			z = (T)vect.z;
		}


		/*=============================================================================
		-- Sets the vector to the specified values.
		=============================================================================*/
		void Set(T x, T y, T z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}


		/*=============================================================================
		-- Adds the values to the vector.
		=============================================================================*/
		void Add(T x, T y, T z)
		{
			this->x += x;
			this->y += y;
			this->z += z;
		}


		/*=============================================================================
		-- Calculates the distance between @this and @vect.
		=============================================================================*/
		double Distance(Vector3D<T> vect)
		{
			return std::sqrt( (vect.x-x)*(vect.x-x) + (vect.y-y)*(vect.y-y) + (vect.z-z)*(vect.z-z));
		}


		/*=============================================================================
		-- Calculates the dot product of @this and @vect.
		=============================================================================*/
		double DotProduct(Vector3D<T> vect)
		{
			return (vect.x*x) + (vect.y*y) + (vect.z*z);
		}


		/*=============================================================================
		-- Calculates the normal of @this on the left or right side based on @leftSide.
		=============================================================================*/
		Vector3D Normal(Vector3D<T> vect, bool leftSide)
		{
			Vector3D normal;

			if (leftSide)
			{
				normal.x = -(vect.y-y);
				normal.y = (vect.x-x);
				normal.z = (vect.z-z); //TODO is this correct?
			}
			else
			{
				normal.x = (vect.y-y);
				normal.y = -(vect.x-x);
				normal.z = (vect.z-z); //TODO is this correct?
			}

			return normal;
		}


		/*=============================================================================
		-- Normalizes @this.
		=============================================================================*/
		void Normalize()
		{
			double length = std::sqrt(x*x + y*y + z*z);
			x /= length;
			y /= length;
			z /= length;
		}


		/*=============================================================================
		-- Calculates a normalized velocity value from @this to @vect.
		=============================================================================*/
		Vector3D Velocity(Vector3D<T> vect)
		{
			Vector3D<T> velocity;

			//calculate x and y velocity
			Vector2D<T> v1 (x, y);				//this as 2D vector (for x and y)
			Vector2D<T> v2 (vect.x, vect.y);	//@vect as 2D vector (for x and y)
			double angle = v1.Angle(v2);

			if (angle < 0.0)
				angle += TWO_PI;

			velocity.x = (T)std::cos(angle*PI_OVER_180);
			velocity.y = (T)std::sin(angle*PI_OVER_180);


			//calculate the z velocity
			v1.Set(y, z);						//this as 2D vector (for y and z)
			v2.Set(vect.y, vect.z);				//@vect as 2D vector (for y and z)

			double zAngle = v1.Angle(v2);
			if (zAngle < 0.0)
				zAngle += TWO_PI;

			velocity.z = (T)std::sin(zAngle);

			return velocity;
		}


		/*=============================================================================
		-- Assignment operators
		=============================================================================*/
		void operator += (const Vector3D<T>& vect)
		{
			x += vect.x;
			y += vect.y;
			z += vect.z;
		}//TODO overload all these to multiply by a single double

		void operator -= (const Vector3D<T>& vect)
		{
			x -= vect.x;
			y -= vect.y;
			z -= vect.z;
		}

		void operator *= (const Vector3D<T>& vect)
		{
			x *= vect.x;
			y *= vect.y;
			z *= vect.z;
		}


		void operator *= (double amount)
		{
			x *= amount;
			y *= amount;
			z *= amount;
		}

		void operator /= (const Vector3D<T>& vect)
		{
			x /= vect.x;
			y /= vect.y;
			z /= vect.z;
		}


		/*=============================================================================
		-- Expressional operators.
		=============================================================================*/
		Vector3D<T> operator + (const Vector3D<T>& vect)
		{
			return Vector3D<T>(x+vect.x, y+vect.y, z+vect.z);
		}

		Vector3D<T> operator + (T n)
		{
			return Vector3D<T>(x+n, y+n, z+n);
		}

		Vector3D<T> operator - (const Vector3D<T>& vect)
		{
			return Vector3D<T>(x-vect.x, y-vect.y, z-vect.z);
		}

		Vector3D<T> operator - (T n)
		{
			return Vector3D<T>(x-n, y-n, z-n);
		}
		
		Vector3D<T> operator  *(const Vector3D<T>& vect)
		{
			return Vector3D(x*vect.x, y*vect.y, z*vect.z);
		}

		Vector3D<T> operator  *(T n)
		{
			return Vector3D<T>(x*n, y*n, z*n);
		}

		Vector3D<T> operator / (const Vector3D<T>& vect)
		{
			return Vector3D<T>(x/vect.x, y/vect.y, z/vect.z);
		}

		Vector3D<T> operator / (T n)
		{
			return Vector3D<T>(x/n, y/n, z/n);
		}

		bool operator == (const Vector3D<T>& vect)
		{
			return (vect.x == x && vect.y == y && vect.z == z);
		}

		bool operator != (const Vector3D<T>& vect)
		{
			return (vect.x != x || vect.y != y || vect.z != z);
    	}
    };
};

#endif

