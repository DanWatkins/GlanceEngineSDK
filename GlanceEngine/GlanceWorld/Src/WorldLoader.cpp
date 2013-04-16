/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: WorldLoader.cpp
=================================================================================================*/
#include "World.h"

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Constructor for World.
        =============================================================================*/
		WorldLoader::WorldLoader()
		{
			mWorld = NULL;
		}


        /*=============================================================================
        -- Main method to load a world file.
        =============================================================================*/
        bool WorldLoader::LoadWorldFromFile(String filepath)
        {
            HMLFile file;
            file.Open(filepath);

            //entity section
            if (file.SeekElementOpen("<entity>"))
            {
                while (!file.AtElementClose("</entity>"))
                {
					//make keyword to make entities in the world
                    if (file.TokenIsDeclaration("#make"))
                    {
                        String templateName = file.SeekGetToken();
                        Vector3D<double> pos = ToVector3D(file.SeekGetToken());

						mWorld->GetEntityManager()->CreateEntity(templateName, pos, file.SeekGetToken());
                    }

					//set keyword to set special parameters
					if (file.TokenIsDeclaration("#set"))
					{
						file.Seek();

						//player identifier to set the handle to the entity the player will reference
						if (file.GetToken() == "player")
						{
							//actual SharedPtr set later 
							mWorld->GetPlayer()->Init(file.SeekGetToken(), mWorld->GetEntityManager());
						}
					}

                    file.Seek();
                }
            }

            //reset the seek position because the map section could have been above the entity 
            file.Reset();

            mWorld->GetMap()->LoadMapDataFromWorldFile(filepath);

			//set the size of the physics environment
			Map map = *mWorld->GetMap();
			double width = map.GetWidth() * map.GetTileset()->GetTileWidth();
			double height = map.GetHeight() * map.GetTileset()->GetTileHeight();
			mWorld->GetPhysicsEnvironment()->SetSize(width, height);

			return true;
        }
    };
};