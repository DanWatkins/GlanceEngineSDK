/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Camera.cpp
=================================================================================================*/
#include "World.h"

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Constructor for Camera.
        =============================================================================*/
        Camera::Camera()
        {
            mScreenX        = 0;
            mScreenY        = 0;
            mViewWidth      = 320;
            mViewHeight     = 240;

            mWorld          = NULL;
			mWindow			= NULL;
        }


		/*=============================================================================
        -- Visualizes the world by first rendering the visible map section, then the
		   visible entities on top of it.
        =============================================================================*/
        void Camera::Draw()
        {
			if (mWorld)
				mWorld->GetMap()->GetTileset()->GetImage()->SetWindow(mWindow);
			else
			{
				gDebug.Error("Bad World object. Cannot draw!", __FUNCTION__);
				return;
			}

			//center the camera on the target if it is valid
			if (!mTarget.expired())
				GoTo( (int)mTarget.lock()->GetX(), (int)mTarget.lock()->GetY(), (int)mTarget.lock()->GetZ() );				

			//TODO
			//mass of data to use!!! Probably should optimize this eventually, but computers are so fast now...so who cares?
            Map *map = mWorld->GetMap();

			Tileset *tileset = map->GetTileset();

			int tileWidth = map->GetTileset()->GetTileWidth();
			int tileHeight = map->GetTileset()->GetTileHeight();
			
			double partialX = std::fmod(mPos.x, (double)tileWidth);
			double partialY = std::fmod(mPos.y, (double)tileHeight);
			int tileX = (int)(mPos.x/tileWidth); //truncate to round down
			int tileY = (int)(mPos.y/tileHeight); //truncate to round down
			float widthTiles = DivideF( (float)mViewWidth, (float)tileWidth )+1.0f;
			float heightTiles = DivideF( (float)mViewHeight, (float)tileHeight )+1.0f;
			
			int x = 0;
			int y = 0;
			int tx = tileX;

			//draw tiles in a left to right, top to bottom fashion
			while (y <= heightTiles  &&  tileY < map->GetHeight())
			{
				//only render if tileY refers to a positive position (there is nothing to render above the map)
				if (tileY >= 0)
				{
					//reset x values
					x = 0;
					tileX = tx;

					while (x <= widthTiles  &&  tileX < map->GetWidth())
					{
						//only render if tileX refers to a positive position (there is nothing to render left of the map
						if (tileX >= 0)
						{
							int posX = RoundDown((double)(x*tileWidth) - partialX);
							int posY = RoundDown((double)(y*tileHeight) - partialY);
							int tile = tileX + (map->GetWidth()*tileY);
							tileset->Draw( (*map)[tile].GetLayer1(), posX, posY );
						}
						tileX++;
						x++;
					}
				}
				tileY++;
				y++;
			}

            //draw every entity in the view area
            EntityManager *manager = mWorld->GetEntityManager();
            for (int n=0; n<manager->EntityCount(); n++)
            {
				double x = (*manager)[n].lock()->GetX() - mPos.x;
				double y = (*manager)[n].lock()->GetY() - mPos.y;
				double z = (*manager)[n].lock()->GetZ() - mPos.z;

                WeakPtr<Entity> e = (*manager)[n];

				e.lock()->Draw( (int)x, (int)(y-z), mWindow); //OPTI
            }


			//draw cached tile groups
			/*std::vector<TileGroup> tileGroups = mWorld->GetMap()->GetCachedTileGroups();
			std::vector<TileGroup>::iterator iter = tileGroups.begin();
			while (iter != tileGroups.end())
			{
				Vector2D<double> pos = (*iter).rect.GetPos()-Vector2D<double>(mPos.x, mPos.y);
				Vector2D<double> oppositePos = Vector2D<double>(pos.x+(*iter).rect.GetWidth(), pos.y+(*iter).rect.GetHeight());

				sf::Shape rect = sf::Shape::Rectangle(sf::Vector2f(pos.x,pos.y), sf::Vector2f(oppositePos.x,oppositePos.y), sf::Color(255,0,0,64));
				mWindow->GetWindow()->Draw(rect);

				iter++;
			}*/
        }


		/*=============================================================================
        -- Adds the specified amounts to the position of the camera in the world.
        =============================================================================*/
        void Camera::Move(double x, double y, double z)
        {
            mPos.x += x;
            mPos.y += y;
			mPos.z += z;
        }


		/*=============================================================================
        -- Adds the specified amounts to the position of the target (if there is one)
		   in the world.
        =============================================================================*/
		void Camera::MoveTarget(double x, double y, double z)
		{
			if (!mTarget.expired())
			{
				mTarget.lock()->Move(x, y, z);
			}
		}


		/*=============================================================================
        -- Direct sets the position of the camera to the specified position in the
		   world. Note the positin refers to the cameras center point, not top left.
        =============================================================================*/
        void Camera::GoTo(double x, double y, double z)
        {
            mPos.x = x - (double)(mViewWidth/2);
            mPos.y = y - (double)(mViewHeight/2) - z; 
        }
    };
};