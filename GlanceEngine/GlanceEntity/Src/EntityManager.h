/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: EntityManager.h
=================================================================================================*/
#include "Entity.h"

#ifndef _ge_ENTITY_MANAGER_H
#define _ge_ENTITY_MANAGER_H

namespace ge
{
    namespace world
    {
        /*=============================================================================
        -- Manages every entity in a game world.
        =============================================================================*/
        class EntityManager
        {
        private:
            std::vector< SharedPtr<Entity> > mEntities;				//list of entities active in the world
            std::vector< SharedPtr<Entity> > mTemplateEntities;		//list of template entities to make regular entities
            int mIdTrack;											//represents the id of the last entity added and is used to keep entities from having the same id
            World *mWorld;											//pointer to the world that uses this manager
			
			//static sorting comparison methods
			static bool _Compare_EntityPosY(SharedPtr<Entity> e1, SharedPtr<Entity> e2);
			static bool _Compare_EntityPosZ(SharedPtr<Entity> e1, SharedPtr<Entity> e2);

        public:
            EntityManager();
            ~EntityManager() {}

            //general
            bool Init();
            void Update(double frameTime);
			void HandleCollisionMessages(std::vector<physics::Collision> collisions);
            void Clean();
            void ClearEntities();
			void SortEntitiesByPosition();

            int EntityCount() { return mEntities.size(); }
			int TemplateEntityCount() { return mTemplateEntities.size(); }
            WeakPtr<Entity> operator[] (unsigned n);
            
            //entity managment
			WeakPtr<Entity> CreateEntity(String templateName, Vector3D<double> pos, String handle);
            WeakPtr<Entity> GetEntity(int id);
			WeakPtr<Entity> GetEntity(String handle);
			WeakPtr<Entity> GetTemplateEntity(int containerIndex) { return mTemplateEntities.at(containerIndex); }
            WeakPtr<Entity> GetTemplateEntity(String name);
			
			void DeleteEntity(int id);
            bool IdAvailable(int id);
            void AddEntity(SharedPtr<Entity> entity);
			void AddTemplateEntity(SharedPtr<Entity> entity);
            void SetWorld(World *world) { mWorld = world; }
            World *GetWorld() { return mWorld; }

        };
    };
};

#endif

