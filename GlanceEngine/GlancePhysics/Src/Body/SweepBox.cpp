/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: SweepBox.cpp
=================================================================================================*/
#include "../Physics.h"

namespace ge
{
    namespace physics
	{
		/*=============================================================================
		-- Constructors for SweepBox.
		=============================================================================*/
		SweepBox::SweepBox()
		{
			mWidth	= 32;
			mLength	= 1;
			_SetType(SWEEPBOX);
		}

		SweepBox::SweepBox(Vector3D<double> pos, double width, double length, double height, Vector3D<double> velocity)
		{
			_SetType(SWEEPBOX);
			SetSize(width, length, height);
			_SetPos(pos);
			_SetVelocity(velocity);
		}


		/*=============================================================================
		-- Returns the center position of the SweepBox.
		=============================================================================*/
		Vector3D<double> SweepBox::GetCenter()
		{
			Vector3D<double> pos(GetPos());
			pos.x += GetWidth()/2;
			pos.y += GetLength()/2;
			pos.z += GetHeight()/2;

			return pos;
		}
	};
};