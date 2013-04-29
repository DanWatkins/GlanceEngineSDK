/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Player.h
=================================================================================================*/
#include "Entity.h"

#ifndef _ge_PLAYER_H
#define _ge_PLAYER_H

namespace ge
{
	namespace world
	{
		/*=============================================================================
		 Encapsulates the abstraction between the player and an actual Actor in a world
		=============================================================================*/
		class Player
		{
		private:
			WeakPtr<Actor> mTarget;			//weak pointer to the actor

		public:
			Player() {}
			~Player() {}

			bool Init(String handle, EntityManager *manager);
			WeakPtr<Actor> GetTarget() { return mTarget; }
			Actor *GetTargetRaw() { return mTarget.lock().get(); }
		};
	};
};

#endif