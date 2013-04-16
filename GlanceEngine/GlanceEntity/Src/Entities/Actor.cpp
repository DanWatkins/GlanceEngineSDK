/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Actor.cpp
=================================================================================================*/
#include "../Entity.h"

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Constructor for Actor.
        =============================================================================*/
        Actor::Actor()
        {
			mInventory = SharedPtr<Inventory>(new Inventory);
            _SetType(ACTOR);
        }


		void Actor::Update()
		{
			Entity::Update();
		}

		/*=============================================================================
        -- Draws the actor.
        =============================================================================*/
        void Actor::Draw(int sx, int sy, Window *window)
        {
			Entity::Draw(sx, sy, window);

            GetAnimationManager().lock()->GetActiveAnimation().lock()->SetWindow(window);	//TODO why are you setting the window every draw call?
			GetAnimationManager().lock()->GetActiveAnimation().lock()->Draw(sx, sy);
        }


		/*=============================================================================
		-- Special actions for colliding with certain enity types.
		=============================================================================*/
		void Actor::ReactToCollision(WeakPtr<Entity> entity)
		{
			if (entity.expired())
				return;

			if (entity.lock()->GetType() == OBJECT)
			{
				WeakPtr<Object> object = WeakPtr<Object>(DynamicPtrCast<Object>(entity.lock()));

				if (object.lock()->GetObtainable())
					HoldObject( object );
			}
		}


		/*=============================================================================
		-- Adds the object to the inventory. Effectively "picking up" the object.
		=============================================================================*/
		void Actor::HoldObject(WeakPtr<Object> object)
		{
			if (!object.expired())
			{
				mInventory->AddObject(object);
				object.lock()->SetHost( GetManager()->GetEntity(GetId()) );
			}
		}


		/*=============================================================================
		-- Drops the object with the id in the inventory if it exists.
		=============================================================================*/
		void Actor::DropObject(int id)
		{
			//place the object on the ground behind the Actor so it does not walk over it right away.
			WeakPtr<Object> object = mInventory->GetObject(id);

			if (!object.expired())
			{
				object.lock()->SetHeld(false);
				object.lock()->SetPos(Vector3D<double>(GetPos().x+32.0, GetPos().y+32.0, GetPos().z+2.0));
				mInventory->RemoveObject(id);
			}
		}


		/*=============================================================================
		-- Drops the active object.
		=============================================================================*/
		void Actor::DropActiveObject()
		{
			if (!mInventory->GetActiveObject().expired())
				DropObject(mInventory->GetActiveObject().lock()->GetId());
		}
    };
};
