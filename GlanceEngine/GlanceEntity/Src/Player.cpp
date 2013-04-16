/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Player.cpp
=================================================================================================*/
#include "Entity.h"

namespace ge
{
	namespace world
	{
		/*=============================================================================
		-- Links the player with an actor and entity manager.
		=============================================================================*/
		bool Player::Init(String handle, EntityManager *manager)
		{
			mTarget = DynamicPtrCast<Actor>(manager->GetEntity(handle).lock());

			return !mTarget.expired();
		}

	};
};
