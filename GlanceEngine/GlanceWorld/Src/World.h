/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: World.h
=================================================================================================*/

//TODO: fix cyclic dependency between entity and world somehow
#include "../../GlanceEntity/Src/Entity.h"

#ifndef _ge_WORLD_H
#define _ge_WORLD_H

/*=============================================================================
-- World forward declarations
=============================================================================*/
namespace ge
{
	namespace world
	{
		struct TileTemplate;
		class Tileset;
		class Map;

		class WorldLoader;
		class World;
		class Camera;

	};
};


/*=============================================================================
-- World related includes
=============================================================================*/
#include "Map/TileSet.h"
#include "Map/Map.h"

#include "WorldLoader.h"
#include "Camera.h"



namespace ge
{
    namespace world
    {
        /*=============================================================================
        -- World class to contain the map, entity manager, and assets.
        =============================================================================*/
        class World
        {
        private:
            EntityManager *mEntityManager;							//entity manager
			physics::Environment *mPhysicsEnvironment;				//main physics environment that the entity manager links with
			physics::Environment *mReservedPhysicsEnvironment;		//seperated physics environment for just template entities
            AssetManager *mAssetManager;							//asset manager to store all assets for the entity system
            Map *mMap;												//map that manages tile data and tileset
			Player *mPlayer;										//represents the active player (links with an Actor entity)
			bool mLoadedDatabase;

        public:
            World();
            ~World() {}

			//general
            bool Init();
            bool LoadWorldFromFile(String filepath);
            void Update(double frameTime);

			//accessor and mutator
            EntityManager *GetEntityManager() { return mEntityManager; }
			physics::Environment *GetPhysicsEnvironment() { return mPhysicsEnvironment; }
			physics::Environment *GetReservedPhysicsEnvironment() { return mReservedPhysicsEnvironment; }
            AssetManager *GetAssetManager() { return mAssetManager; }
            Map *GetMap() { return mMap; }

			void SetPlayer(String id) { mPlayer->Init(id, mEntityManager); }
			Player *GetPlayer() { return mPlayer; }
        };
    };

};



#endif

