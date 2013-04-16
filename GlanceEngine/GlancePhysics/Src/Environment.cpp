/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Environment.cpp
=================================================================================================*/
#include "Physics.h"

namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Constructor for Environment.
		=============================================================================*/
		Environment::Environment()
		{
			mMaxVelocityStep		= 1.0;  //TODO way too large?
			mMinBodySize			= 1.0;
			mIdTrack				= -1;
			mCurrentBodyPartition	= NULL;

			mWidth					= 800000.0;
			mLength					= 800000.0;
			mHeight					= 800000.0;
			mBlockSize				= 4000.0;
			mMaxBodiesPerPartition	= 4;
		}


		/*=============================================================================
		-- Checks for collision between every Body, then handles them appropriately.
		=============================================================================*/
		std::vector<Collision> Environment::Update()
		{			
			std::vector< std::vector< SharedPtr<Body> > > partitions = _GenerateBlocks();

			//list of every collision that occured to be returned for an entity system to use
			std::vector<Collision> collisions;
			
			std::vector< std::vector< SharedPtr<Body> > >::iterator partitionIter = partitions.begin();
			while (partitionIter != partitions.end())
			{
				mCurrentBodyPartition = &(*partitionIter);

				//check every Body for collision with other body
				std::vector< SharedPtr<Body> >::iterator iter = mCurrentBodyPartition->begin();
				while (iter != mCurrentBodyPartition->end())
				{
					//do not check for collision if the Body is not moving or is inactive
					if ((*iter)->GetActive())
					{
						_UpdateGravitationalVelocity(*iter);

						if ((*iter)->GetVelocity() != Vector3D<double>())
						{
							//set the found supporting body flag during update to false because we are
							//just starting the update that checks for a supporting body
							(*iter)->_SetFoundSupportingBodyDuringUpdate(false);

							//if the Body is moving too slow to pass through any object (based on mMaxVelocityStep, so it guarentees nothing)
							if ( std::abs((*iter)->GetVelocity().x) <= mMaxVelocityStep  &&
								 std::abs((*iter)->GetVelocity().y) <= mMaxVelocityStep  &&
								 std::abs((*iter)->GetVelocity().z) <= mMaxVelocityStep )
							{
								_ManageCollisions_Std(*iter, collisions);
							}

							//the Body is moving too fast and could pass through an object
							else
								_ManageCollisions_Inc(*iter, collisions);

							(*iter)->_SetHasSupportingBody( (*iter)->_GetFoundSupportingBodyDuringUpdate() );
						}
					}

					iter++;
				}

				//reset the velocity for every body
				//this must be done after every collision has been handled
				iter = mCurrentBodyPartition->begin();
				while (iter != mCurrentBodyPartition->end())
				{
					(*iter)->_ResetVelocity();

					iter++;
				}

				partitionIter++;
			}

			mCurrentBodyPartition = NULL;

			return collisions;
		}


		/*=============================================================================
		-- Applies more velocity from gravity based on how long the body has been
		   falling, if it is falling.
		=============================================================================*/
		void Environment::_UpdateGravitationalVelocity(WeakPtr<Body> body)
		{
			if (body.lock()->GetRespondsToGravity())
			{
				if (body.lock()->GetHasSupportingBody() == false)
				{
					double fallDuration = body.lock()->GetFallDuration()/1000.0; //convert to seconds
					double verticalVelocityFromGravity = -1.0*fallDuration*fallDuration*ACCELERATION_DUE_TO_GRAVITY_IN_PPS;
					body.lock()->_SetVelocity( body.lock()->GetVelocity() + Vector3D<double>(0.0, 0.0, verticalVelocityFromGravity/5.0) );
				}
			}
		}


		/*=============================================================================
		-- Returns a SharedPtr<Body> to the body with the id.
		=============================================================================*/
		WeakPtr<Body> Environment::GetBody(int id)
		{
			std::vector< SharedPtr<Body> >::iterator iter = mBodies.begin();
			while (iter != mBodies.end())
			{
				if ( (*iter)->GetId() == id )
					return *iter;

				iter++;
			}

			return SharedPtr<Body>();
		}


		/*=============================================================================
		-- Returns a SharedPtr<Box> to a Box with the id. If a Body is found with the
		   right id, but is not a Box, an empty SharedPtr<Box> is returned.
		=============================================================================*/
		WeakPtr<Box> Environment::GetBox(int id)
		{
			SharedPtr<Body> body( GetBody(id) );

			if (body->GetType() == BOX)
				return WeakPtr<Box>(DynamicPtrCast<Box>(body));
			else
				return WeakPtr<Box>();
		}
	};
};