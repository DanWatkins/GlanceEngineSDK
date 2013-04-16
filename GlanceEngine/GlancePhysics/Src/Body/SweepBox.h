/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: SweepBox.h
=================================================================================================*/
#include "../Physics.h"

#ifndef _ge_SWEEP_BOX_H
#define _ge_SWEEP_BOX_H


namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Represents a box of the area a body will occupy fromthe beginning of a
		   movement to the end.
		=============================================================================*/
		class SweepBox : public Body
		{
		private:
			double mWidth, mLength;		//width and height of the box


		public:
			SweepBox();
			SweepBox(Vector3D<double> pos, double width, double length, double height, Vector3D<double> velocity);

			Vector3D<double> GetCenter();
			SharedPtr<SweepBox> GetSweepBox() { return SharedPtr<SweepBox>(); }		//should not be called
			//TODO why should ^that^ not be called?

			//accessor and mutator
			void SetWidth(double width) { mWidth = width; }
			void SetLength(double length) { mLength = length; }
			void SetHeight(double height) { _SetHeight(height); }
			void SetSize(double width, double length, double height) { SetWidth(width); SetLength(length); SetHeight(height); }

			double GetWidth() { return mWidth; }
			double GetRight() { return GetPos().x+mWidth; }

			double GetLength() { return mLength; }
			double GetHeight() { return mHeight; }
			double GetBottom() { return GetPos().y+mHeight; }
		};
	};
};

#endif

