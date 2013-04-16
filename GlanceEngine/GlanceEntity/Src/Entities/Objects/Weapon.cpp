/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Weapon.cpp
=================================================================================================*/
#include "../../Entity.h"

namespace ge
{
    namespace world
    {
		/*=============================================================================
		-- Constructor for Weapon.
		=============================================================================*/
		Weapon::Weapon()
		{
			mProjectileSpeed = 8.0;
			_SetObjectType(WEAPON);
		}


		/*=============================================================================
		-- Creates a projectile and sets the velocity of it to a certain direction.
		=============================================================================*/
		void Weapon::LaunchProjectile(Vector3D<double> sourcePos, Vector3D<double> targetPos)
		{
			//take a round from the ammo clip
			mAmmo.lock()->UseRounds(1);

			//create the round as a projectile
			WeakPtr<Object> object = DynamicPtrCast<Object>(GetManager()->CreateEntity(mAmmo.lock()->GetProjectileName(), sourcePos, "NULL").lock());
			object.lock()->SetProjectileExclusive(true);

			//calculate the velocity of the projectile and move the projectile that much
			//note that the projectile will keep this velocity because it is exclusively a projectile
			object.lock()->Move( sourcePos.Velocity(targetPos)*mProjectileSpeed );
		}
	};
};