/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Object.h
=================================================================================================*/
#include "../Entity.h"

#ifndef _ge_OBJECT_H
#define _ge_OBJECT_H

namespace ge
{
    namespace world
    {
		enum ObjectType
		{
			STATIC = 1,
			AMMO = 2,
			WEAPON = 3
		};


		/*=============================================================================
		-- Base object class derived from Entity.
		=============================================================================*/
		class Object : public Entity
		{
		private:
			ObjectType mObjectType;				//type of the object (can this be removed and use typeid() or something?)
			bool mHeld;							//true if it is being held by an Actor and should not be "on" the map
			bool mCombined;						//true if this item shoulded be grouped (ex. "Potion x7")
			bool mObtainable;					//true if this item can be picked up by another entity
			WeakPtr<Entity> mHost;				//host containing the object
			bool mProjectileExclusive;			//if true, the object will be removed after being stationary for a few seconds. And be treated like a bullet or stuff

		protected:
			void _SetObjectType(ObjectType type) { mObjectType = type; }

		public:
			Object();
			~Object() {}

			//general
			void Draw(int sx, int sy, Window *window);

			//accessor and mutator
			ObjectType GetObjectType() { return mObjectType; }
			void SetHeld(bool held);
			bool GetHeld() { return mHeld; }
			void SetCombined(bool combined) { mCombined = combined; }
			bool GetCombined() { return mCombined; }
			void SetHost(WeakPtr<Entity> host) { mHost = host; }
			WeakPtr<Entity> GetHost() { return mHost; }
			void SetProjectileExclusive(bool exclusive);
			bool GetProjectileExclusive() { return mProjectileExclusive; }
			void SetObtainable(bool obtainable) { mObtainable = obtainable; }
			bool GetObtainable() { return mObtainable; }
		};
	};
};

#endif

