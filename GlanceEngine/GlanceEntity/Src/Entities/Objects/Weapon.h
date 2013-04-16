/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Weapon.h
=================================================================================================*/
#include "../../Entity.h"

#ifndef _ge_WEAPON_H
#define _ge_WEAPON_H

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Weapon object to hanle combat.
        =============================================================================*/
		class Weapon : public Object
		{
		private:
			WeakPtr<Ammo> mAmmo;			//weak pointer to the ammo object loaded
			double mProjectileSpeed;		//speed the projectiles launched travel (px per frame)

		public:
			Weapon();
			~Weapon() {}

			void LaunchProjectile(Vector3D<double> sourcePos, Vector3D<double> targetPos);

			//accessor and mutator
			void SetAmmo(WeakPtr<Ammo> ammo) { mAmmo = ammo; }
			WeakPtr<Ammo> GetAmmo() { return mAmmo; }
			void SetProjectileSpeed(double projectileSpeed) { mProjectileSpeed = projectileSpeed; }
			double GetProjectileSpeed() { return mProjectileSpeed; }
		};
	};
};

#endif

