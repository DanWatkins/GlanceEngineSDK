/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Actor.h
=================================================================================================*/
#include "../Entity.h"

#ifndef _ge_ACTOR_H
#define _ge_ACTOR_H

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Actor subclass of Unit. Represents characters and animals.
        =============================================================================*/
        class Actor : public Entity
        {
        private:
			SharedPtr<Inventory> mInventory;	//inventory for the Actor

        public:
            Actor();
            ~Actor() {}

			//general
			void Update();
            void Draw(int sx, int sy, Window *window);
			void ReactToCollision(WeakPtr<Entity> entity);

			//object managment
			void HoldObject(WeakPtr<Object> object);
			void DropObject(int id);
			void DropActiveObject();

			//accessor and mutator
			WeakPtr<Inventory> GetInventory() { return mInventory; }
        };
    };
};

#endif

