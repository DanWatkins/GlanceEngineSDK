/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Cylinder.h
=================================================================================================*/
#include "../Physics.h"

#ifndef _ge_CYLINDER_H
#define _ge_CYLINDER_H


namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Circular body type.
		=============================================================================*/
		class Circle : public Body
		{
		private:
			double mRadius;		//radius of the circle


		protected:
			void _SetRadius(double radius) { mRadius = radius; }


		public:
			friend class Environment;

			Circle() { mRadius = 0.0; _SetType(CYLINDER); }
			Circle(double radius) { _SetRadius(radius); }

			//accessor and mutator
			SharedPtr<SweepBox> GetSweepBox() { return SharedPtr<SweepBox>(); }
			Vector3D<double> GetCenter() { return Vector3D<double>(GetPos().x+mRadius, GetPos().y+mRadius, 0.0); }//TODO
			double GetRadius() { return mRadius; }
		};
	};
};

#endif

		