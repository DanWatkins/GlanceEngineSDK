/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Environment_Create.cpp
=================================================================================================*/
#include "Physics.h"

namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Creates a new Box or Circle by copying @body, but giving the new body the
		   specified position and host id.
		=============================================================================*/
		WeakPtr<Body> Environment::CreateBody(WeakPtr<Body> body, Vector3D<double> pos, int hostId)
		{
			if (!body.expired())
			{
				WeakPtr<Body> primaryBody;

				//Box
				if (body.lock()->GetType() == BOX)
					primaryBody = _CreateBoxFromBox(DynamicPtrCast<Box>(body.lock()));
				//circle
				else if (body.lock()->GetType() == CYLINDER)
					primaryBody = CreateCircle( DynamicPtrCast<Circle>(body.lock())->GetRadius(), pos, hostId);

				//make copies of all the connected bodies (make sure to remove references to all the templates)
				std::vector< WeakPtr<Body> > connectedBodies = primaryBody.lock()->mConnectedBodies;
				primaryBody.lock()->mConnectedBodies.clear();
				std::vector< WeakPtr<Body> >::iterator iter = connectedBodies.begin();
				while (iter != connectedBodies.end())
				{
					WeakPtr<Body> connectedBody = CreateBody((*iter), (*iter).lock()->GetPos(), hostId);
					primaryBody.lock()->ConnectWithBody(connectedBody);

					iter++;
				}

				//set other parameters once all the connected bodies have been situated
				primaryBody.lock()->_SetHostId(hostId);
				primaryBody.lock()->_SetPos(pos);

				return primaryBody;
			}

			return WeakPtr<Body>();
		}


		/*=============================================================================
		-- Creates a
		=============================================================================*/
		WeakPtr<Body> Environment::_CreateBoxFromBox(WeakPtr<Box> box)
		{
			SharedPtr<Box> newBox(new Box(*box.lock().get()));

			mIdTrack++;
			newBox->_SetId(mIdTrack);
			mBodies.push_back(newBox);

			return newBox;
		}


		/*=============================================================================
		-- Creates a new Box body type in the Environment.
		=============================================================================*/
		WeakPtr<Body> Environment::CreateBox(double width, double length, double height, Vector3D<double> pos, int hostId, Vector3D<double> offset)
		{
			//make sure @width and @height meet the minimum size
			if (width < mMinBodySize)
				width = mMinBodySize;
			if (length < mMinBodySize)
				length = mMinBodySize;
			if (height < mMinBodySize)
				height = mMinBodySize;

			//make a new Box and add it
			SharedPtr<Box> box(new Box);
			box->_SetSize(width, length, height);
			box->_SetPos(pos+offset);

			mIdTrack++;
			box->_SetId(mIdTrack);
			box->_SetHostId(hostId);
			mBodies.push_back(box);

			return WeakPtr<Body>(box);
		}


		/*=============================================================================
		-- Creates a new Cirlce body type in the Environment.
		=============================================================================*/
		WeakPtr<Body> Environment::CreateCircle(double radius, Vector3D<double> pos, int hostId)
		{
			//make sure @radius meets the minimum size
			if (radius < mMinBodySize)
				radius = mMinBodySize;

			//make a new Circle and add it
			SharedPtr<Circle> circle(new Circle);
			circle->_SetRadius(radius);
			circle->_SetPos(pos);

			mIdTrack++;
			circle->_SetId(mIdTrack);
			circle->_SetHostId(hostId);
			mBodies.push_back(circle);

			return WeakPtr<Body>(circle);
		}
	};
};
