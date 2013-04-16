/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: World.cpp
=================================================================================================*/
#include "World.h"

namespace ge
{
    namespace world
    {
        /*=============================================================================
        -- Constructor for World.
        =============================================================================*/
        World::World()
        {
            mEntityManager = new EntityManager;
            mEntityManager->SetWorld(this);
			mPhysicsEnvironment = new physics::Environment;
			mReservedPhysicsEnvironment = new physics::Environment;
            mAssetManager = new AssetManager;
            mMap = new Map(this);
			mPlayer = new Player;
			mLoadedDatabase = false;
        }


        /*=============================================================================
        -- Loads the entire entity database.
        =============================================================================*/
        bool World::Init()
        {
            bool error = false;

			if (!mAssetManager->LoadAssetsFromDirectory(DIR_ASSET_GRAPHICS))
                error = true;

            if (!mEntityManager->Init())
                error = true;

			mLoadedDatabase = !error;

            return !error;
        }


        /*=============================================================================
        -- Loads a world file (essentially a wrapper over the WorldLoader class).
        =============================================================================*/
        bool World::LoadWorldFromFile(String filepath)
        {
			//clear out other systems if already loaded
			if (mLoadedDatabase)
			{
				mEntityManager->ClearEntities();
				mPhysicsEnvironment->ClearBodies();

			}


            WorldLoader loader;
            loader.SetWorld(this);
            return loader.LoadWorldFromFile(filepath);
        }


        /*=============================================================================
        -- Updates everything in the world. @frameTime is used to keep entities moving
		   at the same rate regardless of the overall update speed.
        =============================================================================*/
        void World::Update(double frameTime)
        {
				mEntityManager->Update(frameTime);

			//Timer physicsClock;
				std::vector<physics::Collision> collisions = mPhysicsEnvironment->Update();
				mEntityManager->HandleCollisionMessages(collisions);
			//std::cout << "  [Physics Update: " << physicsClock.GetTimeAndEnd() << "ms]" << std::endl;
        }
    };
};