/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: EntityLoader.h
=================================================================================================*/
#include "Entity.h"

#ifndef _ge_ENTITY_LOADER_H
#define _ge_ENTITY_LOADER_H

namespace ge
{
    namespace world
    {
        /*=============================================================================
        -- Loads the entire entity database.
        =============================================================================*/
        class EntityLoader
        {
        private:
            World *mWorld;       //pointer to be set to the template entity list in an EntityManager

        public:
            EntityLoader() { mWorld = NULL; }
            ~EntityLoader() {}
			
            bool LoadEntityDatabase(World *world);
            bool LoadTemplateEntityFromFile(String filepath);

            //file sectors to load
            EntityType Parse_EntityType(HMLFile& file);
            void Parse_Entity_HeaderData(HMLFile& file, SharedPtr<Entity> entity);
			void Parse_Entity_WorldData(HMLFile& file, SharedPtr<Entity> entity);
			void Parse_Entity_GraphicsData(HMLFile& file, SharedPtr<Entity> entity);


			ObjectType Parse_ObjectType(HMLFile& file);
			void Parse_Object_ObjectData(HMLFile& file, SharedPtr<Object> object);
			void Parse_Ammo_AmmoData(HMLFile& file, SharedPtr<Ammo> ammo);
			void Parse_Weapon_WeaponData(HMLFile& file, SharedPtr<Weapon> weapon);
			
        };

    };
};

#endif

