/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Box.h
=================================================================================================*/
#include "../Physics.h"

#ifndef _ge_BOX_H
#define _ge_BOX_H


namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Box body type (axis-aligned bounding box, no rotation).
		=============================================================================*/
		class Box : public Body
		{
		private:
			double mWidth, mLength;
			SharedPtr<SweepBox> mSweepBox;

			//private mutator methods because they can comprimise the physics if used externally from the physics Environment
			void _SetWidth(double width) { mWidth = width; }
			void _SetLength(double length) { mLength = length; }
			void _SetSize(double width, double length, double height) { _SetWidth(width); _SetLength(length); _SetHeight(height); }


		public:
			friend class Environment;

			Box();
			Box(Vector3D<double> pos, double width, double length, double height);
			Box(WeakPtr<SweepBox> sweepBox);

			//general
			bool Contains(Vector3D<double> point);
			bool Contains(double x, double y, double z);
			SharedPtr<SweepBox> GetSweepBox();
			Vector3D<double> GetCenter();
			
			//accessor and mutator
			double GetWidth() { return mWidth; }
			double GetLength() { return mLength; }
		};
	};
};

#endif

