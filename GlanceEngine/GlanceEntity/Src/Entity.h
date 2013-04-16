/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Entity.h
=================================================================================================*/
#include "Constants.h"

#ifndef _ge_ENTITY_H
#define _ge_ENTITY_H

namespace ge
{
	namespace world
	{
        /*=============================================================================
        -- Base entity class to represent things in a game world.
        =============================================================================*/
        class Entity
        {
        private:
            int mId;							//unique id assigned at runtime
            EntityType mType;                   //type of entity (used to identify subclass types such as Actor and Object)
            String mName;						//internal name of the entity used to reference the template entity
            String mDisplayName;				//name of entity displayed in the editor and game
			String mHandle;						//pre-runtime id value. Used for scripting purposes
            bool mAlive;                        //if false, the entity manager will remove the entity on the Clean() call
			World *mWorld;                      //pointer to the world hosting the entity
			double mRotation;					//orientation of the entity on the xy plane in degrees. 0 is east, 90 is north
			SharedPtr<AnimationManager> mAnimationManager;

			//Body information
			Vector3D<double> mBodyAnchor;		//offset to place the Entity's visual position relative to its actual position
			WeakPtr<physics::Body> mBody;		//physics body to represent the unit in the physics enviornment


			//private methods
			Entity& _Copy(const Entity& entity);
			void _CalculateRotation();
			void _UpdateAnimation();


		protected:
			//protected methods only to be used internally and by EntityManager and EntityLoader
			void _SetId(int id) { mId = id; }
			void _SetType(EntityType type) { mType = type; }
			void _SetName(String name) { mName = name; }
			void _SetHandle(String handle) { mHandle = handle; }
			void _SetWorld(World *world) { mWorld = world; }
			void _SetBody(WeakPtr<physics::Body> body) { mBody = body; }
			WeakPtr<physics::Body> _GetBody() { return mBody; }
			void _SetBodyAnchor(Vector3D<double> anchor) { mBodyAnchor = anchor; }


        public:
			friend class EntityManager;
			friend class EntityLoader;

            Entity();
			Entity(const Entity& entity);
			Entity& operator= (const Entity& entity);
			~Entity() {}

            //general
            virtual bool Init() { return true; }
            virtual void Update();
            virtual void Draw(int, int, Window*);
			virtual void ReactToCollision(WeakPtr<Entity> entity) {}

			void Move(double x, double y, double z);
			void Move(Vector3D<double> velocity) { Move(velocity.x, velocity.y, velocity.z); }

            //accessor mutator
            int GetId() { return mId; }
            EntityType GetType() { return mType; }            
            String GetName() { return mName; }
            void SetDisplayName(String name) { mDisplayName = name; }
            String GetDisplayName() { return mDisplayName; }			
			String GetHandle() { return mHandle; }            
            bool GetAlive() { return mAlive; }
			
			void SetPos(Vector3D<double> pos);
			Vector3D<double> GetPos();
			double GetX() { return GetPos().x; }
			double GetY() { return GetPos().y; }
			double GetZ() { return GetPos().z; }
			void SetAlive(bool alive) { mAlive = alive; }

			double GetRotation() { return mRotation; }
			Vector3D<double> GetBodyAnchor() { return mBodyAnchor; }
            
            EntityManager *GetManager();
            World *GetWorld() { return mWorld; }
			WeakPtr<AnimationManager> GetAnimationManager() { return mAnimationManager; }
        };
    };
};


#endif

