/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Collision.cpp
=================================================================================================*/
#include "Physics.h"

namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Constructor for Collision.
		=============================================================================*/
		Collision::Collision(bool areColliding, SharedPtr<Body> b1, SharedPtr<Body> b2)
		{
			mDistance = 0.0;
			mAreColliding = areColliding;
			mB1 = b1;
			mB2 = b2;

			mResolveX = 0.0f;
			mResolveY = 0.0f;
			mResolveZ = 0.0f;
				
			if (mB1 && mB2)
				mDistance = mB1->GetCenter().Distance( mB2->GetCenter() );
			else
				mDistance = 0;
		}


		/*=============================================================================
		-- Swaps the data between @c and @this.
		=============================================================================*/
		void Collision::Swap(Collision& c)
		{
			Collision c2(mAreColliding, mB1, mB2);
			*this = c;
			c = c2;
		}
	};
};