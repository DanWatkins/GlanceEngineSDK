/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Ammo.cpp
=================================================================================================*/
#include "../../Entity.h"

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Constructor for Weapon.
        =============================================================================*/
		Ammo::Ammo()
		{
			mRoundCount = 10;
			_SetObjectType(AMMO);
		}


		/*=============================================================================
        -- Expends a certain number of rounds, and returns the number of rounds
		   successfuly expended. For example: if UseRounds(3) is called, but only 2
		   rounds are left, 2 is returned. If UseRounds(3) is called and 3 rounds are
		   left, 3 is returned.
        =============================================================================*/
		int Ammo::UseRounds(int amount)
		{
			//there are enouph rounds to use as many specified
			if (amount >= mRoundCount)
			{
				mRoundCount -= amount;
				return amount;
			}
			//there are less rounds remaining than specified, so use whatever is left
			else if (mRoundCount > 0)
			{
				int used = mRoundCount;
				mRoundCount = 0;
				return used;
			}
			//there are no rounds remaining or something became invalid (negative value), so none can be used
			else
				return 0;
		}
	};
};