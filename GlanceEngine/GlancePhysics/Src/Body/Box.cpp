/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Box.cpp
=================================================================================================*/
#include "../Physics.h"

namespace ge
{
    namespace physics
	{
		/*=============================================================================
		-- Constructors for Box.
		=============================================================================*/
		Box::Box()
		{
			mWidth		= 32;
			mLength		= 32;
			mHeight		= 32;
			mSweepBox = SharedPtr<SweepBox>(new SweepBox);
			_SetType(BOX);
		}

		Box::Box(Vector3D<double> pos, double width, double length, double height)
		{
			_SetType(BOX);
			_SetPos(pos);
			_SetSize(width, length, height);
			mSweepBox = SharedPtr<SweepBox>(new SweepBox);
		}

		Box::Box(WeakPtr<SweepBox> sweepBox)
		{
			_SetType(BOX);
			_SetId(sweepBox.lock()->GetId());
			_SetPos(sweepBox.lock()->GetPos());
			_SetSize(sweepBox.lock()->GetWidth(), sweepBox.lock()->GetLength(), sweepBox.lock()->GetHeight());
			_SetVelocity(sweepBox.lock()->GetVelocity());//TODO this is probably unecessary, so optimize out
			mSweepBox = SharedPtr<SweepBox>(new SweepBox);
		}


		/*=============================================================================
		-- Returns true if the point (@x,@y) is inside of this Box.
		=============================================================================*/
		bool Box::Contains(Vector3D<double> point)
		{
			return Contains(point.x, point.y, point.z);
		}
		
		bool Box::Contains(double x, double y, double z)
		{
			return ( (x>=GetPos().x) && (x<=GetPos().x+mWidth) && (y>=GetPos().y) && (y<=GetPos().y+mLength) && (z>=GetPos().z) && (z<=GetPos().z+mHeight) );
		}


		/*=============================================================================
		-- Returns a SweepBox that is large enough to contain the whole path the box
		   will take according to its velocity. Note that it does not include the area
		   the Box currently occupies.
		=============================================================================*/
		SharedPtr<SweepBox> Box::GetSweepBox()
		{
			//see if the velocity and dimensions are the same as last time because then we can reuse the sweep box
			if (mSweepBox->GetVelocity() == GetVelocity())
			{
				if (mSweepBox->GetWidth() == mWidth  &&  mSweepBox->GetLength() == mLength  &&  mSweepBox->GetHeight() == mHeight)
					return mSweepBox;
			}

			Vector3D<double> vel(GetVelocity());
			Vector3D<double> nearCorner, farCorner;
			SweepBox *sweepBox = new SweepBox();


			//TODO nearCorner doesn't make any sense after v0.19.0, so rename it to pos or something


			//there is no need to calculate the near and far corners for the z-axis
			sweepBox->SetHeight(std::abs(vel.z));
			double farCornerDirection = 1;  //if 1, the far corner's pos.z is greater than the near corner's pos.z

			if (vel.z < 0.0)
				farCornerDirection = -1;

			Vector3D<double> sweepBoxPos;

			//find the near corner (the pos of the sweep box)
			//TODO optimize this by placing the most frequently configured velocities first
			if (vel.x < 0)
			{
				if (vel.y > 0)
				{
					if (vel.z < 0)
						nearCorner = Vector3D<double>(GetPos().x+vel.x, GetPos().y, GetPos().z+vel.z);
					else
						nearCorner = Vector3D<double>(GetPos().x+vel.x, GetPos().y, GetPos().z);
				}
				//north to west (90-180)
				else
				{
					if (vel.z < 0)
						nearCorner = Vector3D<double>(GetPos().x+vel.x, GetPos().y+vel.y, GetPos().z+vel.z);
					else
						nearCorner = Vector3D<double>(GetPos().x+vel.x, GetPos().y+vel.y, GetPos().z);
				}
			}
			else
			{
				//south to east (90-0)
				if (vel.y > 0)
				{
					if (vel.z < 0)
						nearCorner = Vector3D<double>(GetPos().x, GetPos().y, GetPos().z+vel.z);
					else
						nearCorner = Vector3D<double>(GetPos().x, GetPos().y, GetPos().z);
				}
				//north to east (270-0)
				else
				{
					if (vel.z < 0)
						nearCorner = Vector3D<double>(GetPos().x, GetPos().y+vel.y, GetPos().z+vel.z);
					else
						nearCorner = Vector3D<double>(GetPos().x, GetPos().y+vel.y, GetPos().z);	
				}
			}

			*sweepBox = SweepBox(nearCorner, std::abs(vel.x), std::abs(vel.y), std::abs(vel.z), vel);//TODO it the last parameter even necessary?

			sweepBox->_SetId(GetId());

			//update to the new sweep box
			mSweepBox = SharedPtr<SweepBox>(sweepBox);
			return mSweepBox;	
		}


		/*=============================================================================
		-- Returns the center position of the Box.
		=============================================================================*/
		Vector3D<double> Box::GetCenter()
		{
			Vector3D<double> pos(GetPos());
			pos.x += GetWidth()/2;
			pos.y += GetLength()/2;
			pos.z += GetHeight()/2;

			return pos;
		}
	};
};