/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Ammo.h
=================================================================================================*/
#include "../../Entity.h"

#ifndef _ge_AMMO_H
#define _ge_AMMO_H

namespace ge
{
    namespace world
    {
		/*=============================================================================
		-- Ammo object to keep track of how many rounds are left in a clip.
		=============================================================================*/
		class Ammo : public Object
		{
		private:
			int mRoundCount;			//number of shots left
			String mProjectileName;		//name of the template for the projectile (an object)


		public:
			Ammo();
			~Ammo() {}

			//general
			int UseRounds(int amount);
			void FillRoundsTo(int numberOfRounds) { mRoundCount = numberOfRounds; }

			int GetRoundCount() { return mRoundCount; }
			void SetProjectileName(String name) { mProjectileName = name; }
			String GetProjectileName() { return mProjectileName; }
		};
	};
};

#endif

