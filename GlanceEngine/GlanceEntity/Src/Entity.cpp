/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Entity.cpp
=================================================================================================*/
#include "Entity.h"

namespace ge
{
    namespace world
    {
        /*=============================================================================
        -- Constructors for Entity
        =============================================================================*/
        Entity::Entity()
        {
            mId					= UNDEFINED_ENTITY_ID;
            mType				= UNDEFINED;
			mName				= "";
			mDisplayName		= "untitled_entity";
			mHandle				= "";
            mAlive				= true;
			mWorld				= NULL;
			mRotation			= 0;
			mAnimationManager	= SharedPtr<AnimationManager>(new AnimationManager);
        }

		//reusable copy method
		Entity& Entity::_Copy(const Entity& entity)
		{
			mId							= entity.mId;
			mType						= entity.mType;
			mName						= entity.mName;
			mDisplayName				= entity.mDisplayName;
			mHandle						= entity.mHandle;
			mAlive						= entity.mAlive;
			mWorld						= entity.mWorld;
			mRotation					= entity.mRotation;
			*mAnimationManager.get()	= *entity.mAnimationManager.get(); //deep copy

			mBodyAnchor					= entity.mBodyAnchor;
			mBody						= entity.mBody;

			return *this;
		}

		//copy constructor
		Entity::Entity(const Entity& entity)
		{
			_Copy(entity);
		}

		//assignment operator
		Entity& Entity::operator= (const Entity& entity)
		{
			return _Copy(entity);
		}


		/*=============================================================================
        -- Adds the velocity vector to the body. the physics Environment will determine
		   if the entity can actually move (consideres the net velocity, which is
		   basically the combination of all Move() calls).
        =============================================================================*/
		void Entity::Move(double x, double y, double z)
		{
			mBody.lock()->AddVelocity(x, y, z);
		}


		/*=============================================================================
        -- Returns the entity manager hosting the entity.
        =============================================================================*/
        EntityManager *Entity::GetManager()
        {
            return mWorld->GetEntityManager();
        }


		/*=============================================================================
		-- Setter and getter for the position.
		=============================================================================*/
		void Entity::SetPos(Vector3D<double> pos)
		{
			if (!mBody.expired())
				mBody.lock()->RequestPos(pos.x, pos.y, pos.z);
		}


		/*=============================================================================
		-- Returns the position of the entity's body.
		=============================================================================*/
		Vector3D<double> Entity::GetPos()
		{
			if (!mBody .expired())
				return mBody.lock()->GetPos();

			return Vector3D<double>();
		}



		/*=============================================================================
		-- Updates Entity specific stuff. Should be called by all virtual overides from
		   derived classes.
		=============================================================================*/
		void Entity::Update()
		{
			_CalculateRotation();
			_UpdateAnimation();
		};


		/*=============================================================================
		-- Draws generic entity stuff like shadows. Should be called by all virtual
		   overides from derived classes.
		=============================================================================*/
		void Entity::Draw(int sx, int sy, Window *window)
		{
			//does the shadow represent a rectangular prism?
			if (_GetBody().lock()->GetType() == physics::BOX)
			{
				double width = DynamicPtrCast<physics::Box>(_GetBody().lock())->GetWidth();
				double height = DynamicPtrCast<physics::Box>(_GetBody().lock())->GetLength(); //this is height on a 2D coordinate system

				float x = sx;
				float y = sy+(height/2)+_GetBody().lock()->GetPos().z;
				float x2 = x+width;
				float y2 = y+height + (_GetBody().lock()->GetHeight() * 0.45); //take into consideration how tall the entity is (taller means longer shadow)

				sf::Shape b1 = sf::Shape::Rectangle(x, y, x2, y2, sf::Color(0,0,0,75), 0.0f, sf::Color(0,0,0,0));
				window->GetWindow()->Draw(b1);
			}
		}


		/*=============================================================================
		-- Calculates the xy rotation based on velocity.
		=============================================================================*/
		void Entity::_CalculateRotation()
		{
			//calculate the rotation  //todo clean up
			Vector3D<double> velocity3d(mBody.lock()->GetVelocity());
			Vector2D<double> velocity2d(velocity3d.x, velocity3d.y);
			Vector2D<double> origin(0.0,0.0);

			//if the entity is not moving, its rotation does not change
			if (mBody.lock()->GetVelocity() == Vector3D<double>(0.0,0.0,0.0))
				return;

			//flip the y-velocity so it works with regular cartesian coordinate system
			//TODO why doesn't Vector::Angle work with the inverted cartesian coordinate system?
			velocity2d.y *= -1;

			mRotation = origin.Angle(velocity2d);
			mRotation = ge::RadToDeg(mRotation);

			//reduce the rotation
			if (mRotation > 360.0)
				mRotation -= 360.0;
			if (mRotation < 0.0)
				mRotation += 360.0;
		}


		/*=============================================================================
		-- Determines the right animation to display.
		=============================================================================*/
		void Entity::_UpdateAnimation()
		{
			mAnimationManager->GetActiveAnimation().lock()->SetCurrentRotationInDegrees(mRotation);

			//switch to the appropriate animation
			if (mBody.lock()->GetVelocity() == Vector3D<double>(0.0,0.0,0.0))
				mAnimationManager->SwitchToAnimation(stance::Standing);
			else
				mAnimationManager->SwitchToAnimation(stance::Walking);
		}
	};
};
