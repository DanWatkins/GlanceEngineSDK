/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Collision.h
=================================================================================================*/
#include "Physics.h"

#ifndef _ge_COLLISION_H
#define _ge_COLLISION_H

namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Holds data about a collision. (Weather one occured between the two Body
		   objects is not the responsibility of Collision).
		=============================================================================*/
		class Collision
		{
		private:
			bool mAreColliding;						//true if the two bodies are colliding
			double mDistance;						//distance between the two bodies
			SharedPtr<Body> mB1;					//first body
			SharedPtr<Body> mB2;					//second body
			double mResolveX, mResolveY, mResolveZ;	//1 or -1 mean a resolution needs to be made on that axis. the direction of resolution is specified by the sign

		public:
			Collision (bool areColliding, SharedPtr<Body> b1 = SharedPtr<Body>(), SharedPtr<Body> b2 = SharedPtr<Body>());
			void Swap(Collision& c);

			bool AreColliding() { return mAreColliding; }
			double GetDistance() { return mDistance; }
			SharedPtr<Body> GetB1() { return mB1; }
			SharedPtr<Body> GetB2() { return mB2; }

			void SetResolveX(double resolveX) { mResolveX = resolveX; }
			double GetResolveX() { return mResolveX; }
			void SetResolveY(double resolveY) { mResolveY = resolveY; }
			double GetResolveY() { return mResolveY; }
			void SetResolveZ(double resolveZ) { mResolveZ = resolveZ; }
			double GetResolveZ() { return mResolveZ; }
		};
	};
};

#endif

