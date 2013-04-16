/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: EntityManager.cpp
=================================================================================================*/
#include "Entity.h"

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Constructor for EntityManager.
        =============================================================================*/
		EntityManager::EntityManager()
		{
			mIdTrack = -1;
			mWorld = NULL;
		}


        /*=============================================================================
        -- Returns a weak pointer to the entity with the @id contained in the manager.
        =============================================================================*/
        WeakPtr<Entity> EntityManager::GetEntity(int id)
        {
            std::vector< SharedPtr<Entity> >::iterator iter = mEntities.begin();
            while (iter != mEntities.end())
            {
                if ( (*iter)->GetId() == id )
					return SharedPtr<Entity>(*iter);

                iter++;
            }

            return SharedPtr<Entity>();
        }


        /*=============================================================================
        -- Returns a weak pointer to a template entity with @name.
        =============================================================================*/
        WeakPtr<Entity> EntityManager::GetTemplateEntity(String name)
        {
            SharedPtr<Entity> entity;
            std::vector< SharedPtr<Entity> >::iterator iter = mTemplateEntities.begin();
            while (iter != mTemplateEntities.end())
            {
                if ( (*iter)->GetName() == name )
					return SharedPtr<Entity>(*iter);

                iter++;
            }

            return WeakPtr<Entity>();
        }


		/*=============================================================================
        -- Returns a weak pointer to a template entity with @handle.
        =============================================================================*/
		WeakPtr<Entity> EntityManager::GetEntity(String handle)
		{
			std::vector< SharedPtr<Entity> >::iterator iter = mEntities.begin();
			while (iter != mEntities.end())
			{
				if ( (*iter)->GetHandle() == handle )
					return SharedPtr<Entity>(*iter);

				iter++;
			}

			return SharedPtr<Entity>();
		}


		/*=============================================================================
        -- Removes an entity from the entity list. Note, that if there are any other
		   SharedPtr<>s sharing the same object as the one deleted, the object will
		   remain in memory because those SharedPtr<>s still own it.
        =============================================================================*/
		void EntityManager::DeleteEntity(int id)
		{
			std::vector< SharedPtr<Entity> >::iterator iter = mEntities.begin();
			while (iter != mEntities.end())
			{
				if ( (*iter)->GetId() == id )
				{
					mEntities.erase(iter);
					return;
				}
				iter++;
			}
		}


        /*=============================================================================
        -- Returns true if no other entities have @id.
        =============================================================================*/
        bool EntityManager::IdAvailable(int id)
        {
            std::vector< SharedPtr<Entity> >::iterator iter = mEntities.begin();
            while (iter != mEntities.end())
            {
                if ( (*iter)->GetId() == id )
                    return false;

                iter++;
            };

            return true;
        }


        /*=============================================================================
        -- Adds a shared pointer to an entity to @mEntities.
        =============================================================================*/
        void EntityManager::AddEntity(SharedPtr<Entity> entity)
        {
            mIdTrack++;
            entity->_SetId(mIdTrack);
            mEntities.push_back(entity);
        }

		void EntityManager::AddTemplateEntity(SharedPtr<Entity> entity)
		{
			entity->_SetId(TEMPLATE_ENTITY_ID);
			mTemplateEntities.push_back(entity);
		}


        /*=============================================================================
        -- Initializes the entity system.
        =============================================================================*/
        bool EntityManager::Init()
        {
            world::EntityLoader loader;
            return loader.LoadEntityDatabase(mWorld);
        }


        /*=============================================================================
        -- Updates every entity in the manager and cleans.
        =============================================================================*/
        void EntityManager::Update(double frameTime)
        {
            Clean();

			//update every entity
            std::vector< SharedPtr<Entity> >::iterator iter = mEntities.begin();
            while (iter != mEntities.end())
            {
				//adjust the velocity so it is moving at a constant rate based on @frameTime
				(*iter)->_GetBody().lock()->AdjustVelocityByMultiplier(frameTime);

                (*iter)->Update();

                iter++;
            }

			//sort every entity so the camera renders everything correctly
			SortEntitiesByPosition();
        }


		/*=============================================================================
        -- Goes through every collision found and lets the first body know it collided
		   with the second body.
        =============================================================================*/
		void EntityManager::HandleCollisionMessages(std::vector<physics::Collision> collisions)
		{
			std::vector<physics::Collision>::iterator iter = collisions.begin();
			while (iter != collisions.end())
			{
				WeakPtr<Entity> e1 = GetEntity( (*iter).GetB1()->GetHostId() );
				WeakPtr<Entity> e2 = GetEntity( (*iter).GetB2()->GetHostId() );

				e1.lock()->ReactToCollision( e2 );
				//TODO shouldn't the second body also know about the collision?
				
				iter++;
			}
		}


        /*=============================================================================
        -- Removes entities if they are not alive.
        =============================================================================*/
        void EntityManager::Clean()
        {
            std::vector< SharedPtr<Entity> >::iterator iter = mEntities.begin();
            while (iter != mEntities.end())
            {
                if ( (*iter)->GetAlive() == false )
                {
                    //remove the entity from the manager list
                    mEntities.erase(iter);  //TODO is it safe to continue iteration after removing the entity?
                    iter++;
                }

                iter++;
            }
        }


        /*=============================================================================
        -- Removes every entity in the manager.
        =============================================================================*/
        void EntityManager::ClearEntities()
        {
            mEntities.clear();
        }


		/*=============================================================================
        -- Sorts every entity based on their Y-axis position. Entities with lower
		   Y-axis positions are sorted to the end of the entity list. Then, entities
		   with lower Z-axis positions are sorted to the end, yet maintaining the same
		   relative positions determined in the Y-axis sort.
        =============================================================================*/
		void EntityManager::SortEntitiesByPosition()
		{
			if (mEntities.size() >= 2)
			{
				bool doneSortingYPos = false;
				while (!doneSortingYPos)
				{
					std::vector< SharedPtr<Entity> >::iterator iter1 = mEntities.begin();
					std::vector< SharedPtr<Entity> >::iterator iter2 = mEntities.begin();
					iter2++;
					bool foundConflict = false;	//true if entities needed to be swapped (means the loop will need to make another pass)

					while (iter2 != mEntities.end())
					{
						//sort on y-axis?
						if ((*iter1)->GetPos().y > (*iter2)->GetPos().y)
						{
							std::swap(*iter1, *iter2);
							foundConflict = true;
							break;
						}

						//advance both iters
						iter1++;
						iter2++;
					}

					doneSortingYPos = !foundConflict;
				}


				//sort on the z-axis
				bool doneSortingZPos = false;
				while (!doneSortingZPos)
				{
					std::vector< SharedPtr<Entity> >::iterator iter1 = mEntities.begin();
					std::vector< SharedPtr<Entity> >::iterator iter2 = mEntities.begin();
					iter2++;
					bool foundConflict = false;

					while (iter2 != mEntities.end())
					{
						//iter1 must have a z greater than iter2 plus iter2's height
						if ((*iter1)->GetPos().z > (*iter2)->GetPos().z+(*iter2)->_GetBody().lock()->GetHeight())
						{
							std::swap(*iter1, *iter2);
							foundConflict = true;
							break;
						}

						iter1++;
						iter2++;
					}

					doneSortingZPos = !foundConflict;
				}
			}
		}


		/*=============================================================================
        -- Creates a new entity given the template entity's name and the world position.
        =============================================================================*/
        WeakPtr<Entity> EntityManager::CreateEntity(String templateName, Vector3D<double> pos, String handle)
        {
			WeakPtr<Entity> entity( mWorld->GetEntityManager()->GetTemplateEntity(templateName) );

			if (entity.expired())
				gDebug.Error( String()+"Could not create "+templateName+" entity because there is no template" );
			else if (entity.lock()->GetType() == ACTOR)
			{
				SharedPtr<Actor> actor( new Actor );
				*actor.get() = *DynamicPtrCast<Actor>(entity.lock()).get();
				actor->_SetHandle(handle);

				AddEntity( actor );

				//make a new body in the physics environment and link this entity with it
				actor->_SetBody(mWorld->GetPhysicsEnvironment()->CreateBody( actor->_GetBody(), pos, actor->GetId()));

				return WeakPtr<Entity>(actor);
			}
			
			else if (entity.lock()->GetType() == OBJECT)
			{
				SharedPtr<Object> object(new Object);
				*object.get() = *DynamicPtrCast<Object>(entity.lock()).get();
				object->_SetHandle(handle);

				AddEntity( object );

				//make a new body in the physics enbironment and link this entity with it
				object->_SetBody(mWorld->GetPhysicsEnvironment()->CreateBody( object->_GetBody(), pos, object->GetId()));

				return WeakPtr<Entity>(object);
			}

			return WeakPtr<Entity>();
        }


		/*=============================================================================
        -- Overloaded operator to access the nth entity in the entity list.
        =============================================================================*/
        WeakPtr<Entity> EntityManager::operator[] (unsigned n)
        {
			return mEntities[n];
        }
    };
};