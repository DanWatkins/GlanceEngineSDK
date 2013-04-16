/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: WorldLoader.h
=================================================================================================*/
#include "World.h"

#ifndef _ge_WORLD_LOADER_H
#define _ge_WORLD_LOADER_H

namespace ge
{
    namespace world
    {
        class World;

        /*=============================================================================
        -- Deserializes a world file.
        =============================================================================*/
        class WorldLoader
        {
        private:
            World *mWorld;		//pointer to world object to deserialize data to

        public:
			WorldLoader();
            ~WorldLoader() {}

			//general
            bool LoadWorldFromFile(String filepath);

			//accessor and mutator
            void SetWorld(World *world) { mWorld = world; }
            World *GetWorld() { return mWorld; }
        };
    };
};

#endif

