/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Body.cpp
=================================================================================================*/
#include "Physics.h"

namespace ge
{
    namespace physics
	{
		/*=============================================================================
		-- Constructors for Body.
		=============================================================================*/
		Body::Body()
		{
			mId									= -1;
			mHostId								= -1;
			mType								= UNDEFINED;
			mActive								= true;
			mSaveVelocity						= false;
			mFoundSupportingBodyDuringUpdate	= false;
			mHasSupportingBody					= true;	//must be true because gravity cannot take effect right away (logic issue)
			mRespondsToGravity					= false;
			mHeight								= 1.0;
		}


		/*=============================================================================
		-- Adds a velocity vector to the current velocity of the Body.
		=============================================================================*/
		void Body::AddVelocity(Vector3D<double> vect)
		{
			mVelocity += vect;
		}

		void Body::AddVelocity(double x, double y, double z)
		{
			mVelocity.Add(x, y, z);
		}


		/*=============================================================================
		-- Subtracts a velocity vector from the current velocity of the Body.
		=============================================================================*/
		void Body::SubtractVelocity()
		{
			mPos -= mVelocity;
		}


		/*=============================================================================
		-- Sets the velocity of the Body to zero only if @mSaveVelocity is false. This
		   is used to prevent velocity from accumulating over frames when
		   _Applyvelocity() is called.
		=============================================================================*/
		void Body::_ResetVelocity()  //TODO seems like this should be private?
		{
			if (!mSaveVelocity)
				mVelocity.Set(0.0, 0.0, 0.0);
		}


		/*=============================================================================
		-- Attempts to reposition the body to the specified location if available.
		   At the moment, it simply sets the position without checking for collision
		   or any other attempts to keeping things realistic.
		=============================================================================*/
		void Body::RequestPos(double x, double y, double z)
		{
			_SetPos(x, y, z);
		}

		void Body::ConnectWithBody(WeakPtr<Body> body)
		{
			//make sure there is not already a body with the same id
			bool duplicate = false;
			std::vector< WeakPtr<Body> >::iterator iter = mConnectedBodies.begin();
			while (iter != mConnectedBodies.end())
			{
				if ( (*iter).lock()->GetId() == body.lock()->GetId() )
					duplicate = true;

				iter++;
			}

			if (!duplicate)
			{
				mConnectedBodies.push_back(body);
				body.lock()->_SetHostId(GetHostId());
				body.lock()->SetSaveVelocity(GetSaveVelocity());
			}
		}







		void Body::_ApplyVelocity()
		{
			//apply velocity for all connected 
			/*std::vector< WeakPtr<Body> >::iterator iter = mConnectedBodies.begin();
			while (iter != mConnectedBodies.end())
			{
				(*iter).lock()->mHostId = id;

				iter++;
			} TODO can't really do this? */

			mPos += mVelocity;
		} 
		
		

		void Body::_SetHostId(int id)
		{
			//set host id for all connected bodies
			std::vector< WeakPtr<Body> >::iterator iter = mConnectedBodies.begin();
			while (iter != mConnectedBodies.end())
			{
				(*iter).lock()->mHostId = id;

				iter++;
			}

			mHostId = id;
		}


		void Body::_SetPos(Vector3D<double> pos)
		{
			//set pos of connected bodies and maintain relative positions
			std::vector< WeakPtr<Body> >::iterator iter = mConnectedBodies.begin();
			while (iter != mConnectedBodies.end())
			{
				(*iter).lock()->mPos = pos-GetPos()+(*iter).lock()->GetPos();

				iter++;
			}

			mPos = pos;
		}


		void Body::_SetPos(double x, double y, double z)
		{
			_SetPos( Vector3D<double>(x, y, z) );
		}


		void Body::_SetX(double x)
		{
			_SetPos(x, mPos.y, mPos.z);
		}


		void Body::_AddX(double x)
		{
			_SetPos(mPos.x+x, mPos.y, mPos.z);
		}


		void Body::_SetY(double y)
		{
			_SetPos(mPos.x, y, mPos.z);
		}


		void Body::_AddY(double y)
		{
			_SetPos(mPos.x, mPos.y+y, mPos.z);
		}


		void Body::_SetZ(double z)
		{
			_SetPos(mPos.x, mPos.y, z);
		}


		void Body::_AddZ(double z)
		{
			_SetPos(mPos.x, mPos.y, mPos.z+z);
		}


		void Body::_SetVelocity(Vector3D<double> velocity)
		{
			//set velocity of all connected bodies
			std::vector< WeakPtr<Body> >::iterator iter = mConnectedBodies.begin();
			while (iter != mConnectedBodies.end())
			{
				(*iter).lock()->mVelocity = velocity;

				iter++;
			}

			mVelocity = velocity;
		}


		void Body::_SetHasSupportingBody(bool hasSupportingBody)
		{
			//should the fall duration be reset?
			if (mHasSupportingBody && !hasSupportingBody)
				_ResetFallDuration();

			mHasSupportingBody = hasSupportingBody;
		}
	};
};