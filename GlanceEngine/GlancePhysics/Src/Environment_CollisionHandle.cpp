/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Environment_CollisionHandle.cpp
=================================================================================================*/
#include "Physics.h"

namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Handles a collision between two Box objects that are colliding.
		=============================================================================*/
		void Environment::_HandleCollision(Collision collision)
		{
			SharedPtr<Body> b1 = collision.GetB1();

			_DetermineCollisionResponse(DynamicPtrCast<Box>(collision.GetB1()), DynamicPtrCast<Box>(collision.GetB2()), collision);

			b1->_AddX(collision.GetResolveX());
			b1->_AddY(collision.GetResolveY());
			b1->_AddZ(collision.GetResolveZ());
		}
	};
};