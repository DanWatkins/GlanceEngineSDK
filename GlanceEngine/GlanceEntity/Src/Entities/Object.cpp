/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Object.cpp
=================================================================================================*/
#include "../Entity.h"

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Constructor for AObject.
        =============================================================================*/
        Object::Object()
        {
			mHeld					= false;
			mCombined				= false;
			mProjectileExclusive	= false;
			mObtainable				= false;

            _SetType(OBJECT);
			_SetObjectType(STATIC);
        }


		/*=============================================================================
        -- Draws the actor.
        =============================================================================*/
        void Object::Draw(int sx, int sy, Window *window)
        {
			if (!mHeld)
			{
				Entity::Draw(sx, sy, window);


				GetAnimationManager().lock()->GetActiveAnimation().lock()->SetWindow(window);
				GetAnimationManager().lock()->GetActiveAnimation().lock()->Draw(sx, sy);

				/*//draw the bodies as white rectangles
				float x = sx;
				float y = sy;
				float width = DynamicPtrCast<physics::Box>(_GetBody().lock())->GetWidth();
				float height = DynamicPtrCast<physics::Box>(_GetBody().lock())->GetLength();
				sf::Shape b1 = sf::Shape::Rectangle(x, y, x+width, y+height, sf::Color(255,0,0,85), 0.0f, sf::Color(0,0,0,0));
				window->GetWindow()->Draw(b1);

				for (int n=0; n<_GetBody().lock()->GetConnectedBodiesCount(); n++)
				{
					SharedPtr<physics::Box> box = DynamicPtrCast<physics::Box>(_GetBody().lock()->GetConnectedBody(n).lock());

					float x2 = sx - _GetBody().lock()->GetPos().x + box->GetPos().x;
					float y2 = sy - _GetBody().lock()->GetPos().y + box->GetPos().y;
					float width2 = box->GetWidth();
					float height2 = box->GetLength();
					sf::Shape b2 = sf::Shape::Rectangle(x2, y2, x2+width2, y2+height2, sf::Color(255,0,0,85), 0.0f, sf::Color(0,0,0,0));

					window->GetWindow()->Draw(b2);
				}*/
			}
        }

		/*=============================================================================
		-- Sets if the object is held.
		=============================================================================*/
		void Object::SetHeld(bool held)
		{
			mHeld = held;
			_GetBody().lock()->SetActive(!held);
		}


		/*=============================================================================
		-- Sets if this is supposed to be a projectile.
		=============================================================================*/
		void Object::SetProjectileExclusive(bool exclusive)
		{
			mProjectileExclusive = exclusive;
			_GetBody().lock()->SetSaveVelocity(exclusive);
		}
	};
};