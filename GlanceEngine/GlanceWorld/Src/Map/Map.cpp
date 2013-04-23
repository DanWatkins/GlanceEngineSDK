/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Map.cpp
=================================================================================================*/
#include "../World.h"

namespace ge
{
    namespace world
    {
		Tile::Tile()
        {
			mId = -1;
            mLayer1 = 0;
            mLayer2 = -1; //negative values indicate to not draw anything for that tile.
			mCollidable = false;
			mAddedToGroup = false;
        }


		void Tile::SetLayer(short layer, TileTemplate tileTemplate)
		{
			if (layer == 1)
				mLayer1 = tileTemplate.mRef;
			else if (layer == 2)
				mLayer2 = tileTemplate.mRef;

			//take only the most restrictive collision of all layers
			if (tileTemplate.mCollidable)
				mCollidable = true;
		}

		Map::Map(World *world)
        {
            mWidth  = 128;
            mHeight = 128;
			mWorld	= world;
        }

        /*=============================================================================
        -- Appends @tile to the end of the tile data list if there is room.
        =============================================================================*/
        void Map::AddTile(Tile tile)
        {
            if ( (unsigned)(mWidth*mHeight) > mTileData.size() )
            {
                mTileData.push_back(tile);
            }
        }


		/*=============================================================================
        -- 
        =============================================================================*/
		Tile Map::operator[](unsigned tile)
		{
			Tile returnTile;

			if (mTileData.size() > tile)
				returnTile = mTileData.at(tile);

			return returnTile;
			
		}


        /*=============================================================================
        -- Loads the map portion of a world file.
        =============================================================================*/
        bool Map::LoadMapDataFromWorldFile(String filepath)
        {
            bool error = false;
            HMLFile file;
            file.Open(filepath);

			mTileData.clear();

            if (file.SeekElementOpen("<map>"))
            {
                file.Seek();

                while (!file.AtElementClose("</map>"))
                {
                    if (file.TokenIsDeclaration("#dimensions"))
                    {
                        Vector2D<double> dim = ToVector2D(file.SeekGetToken());
                        mWidth = Round(dim.x);
                        mHeight = Round(dim.y);
                        
                    }
                    else if (file.TokenIsDeclaration("#tileset"))
                    {
						if (!mTileset.LoadTilesetFromFile(file.SeekGetToken()))
                            error = true;
                    }
                    else if (file.SeekElementOpen("<layer1>"))
                    {
                        int n=0;
                        int max=mWidth*mHeight;
                        file.Seek();

                        while (!file.AtElementClose("</layer1>"))
                        {
                            if (n < max)
                            {
                                Tile tile;
								tile.SetLayer(1, mTileset.GetTileTemplate(ToInt(file.GetToken())));
								tile.SetId(n);
                                mTileData.push_back(tile);


                            }

                            n++;
                            file.Seek();
                        }
                    }

                    file.Seek();
                }
            }

			_CreatePhysicBodies();

            return !error;
		}


		/*=============================================================================
        -- Groups all of the tiles into large collision bodies. This is for
		   optimization purposes.
        =============================================================================*/
		std::vector<TileGroup> Map::_GenerateTileGroups()
		{
			std::vector<Tile>::iterator iter = mTileData.begin();
			std::vector<TileGroup> tileGroups;
			int tilesAddedToGroups = 0;
			int tilesToAddToGroups  = GetWidth()*GetHeight();
			bool rowFailure = false;


			//go left to right in the array of tiles and group any tiles that are the same
			//then extrude the sequence down and only group the same tile section below if
			//all of the tiles are the same as the first. keep repeating the search until
			//an odd tile is found
			while (tilesAddedToGroups != tilesToAddToGroups)
			{
				Rect<double> currentGroup;
				iter = mTileData.begin();
				int startX = 0;
				int endX = GetWidth();
				short currentLayer1Criteria = -1; //refers to the layer1 property of the Tile class. (all tiles in a group must have the same one)
				bool endGroup = false;
				bool collidable = false;

				for (int y=0; (y<GetHeight() && !endGroup); y++)
				{
					rowFailure = false; //true if a bad tile has been found
					int x = startX; //row iterator
					int tilesAddedFromCurrentRow = 0;

					iter = mTileData.begin();
					iter += (x+GetWidth()*y);

					for (x=startX; (x<endX && !rowFailure); x++)
					{
						if (!(*iter).GetAddedToGroup()  &&  (currentLayer1Criteria == -1 || currentLayer1Criteria == (*iter).GetLayer1()))
						{
							tilesAddedToGroups++;
							tilesAddedFromCurrentRow++;
							(*iter).SetAddedToGroup(true);

							//mark @startX, set the position, and set the layer1 criteria if this is the first tile added
							if (currentGroup.GetWidth() == 0)
							{
								startX = x;
								currentGroup.SetPos(x*mTileset.GetTileWidth(), y*mTileset.GetTileHeight());
								currentLayer1Criteria = (*iter).GetLayer1();
							}

							//if this is the first row to @currentGroup, then increase the width
							if (currentGroup.GetHeight() == 0)
								currentGroup.SetWidth(currentGroup.GetWidth() + mTileset.GetTileWidth());
						}
						//this is only a row failure if the first tile has already been added
						//and the first tile for this row has been found
						else if (currentGroup.GetWidth() != 0)
							rowFailure = true;

						iter++;
					}

					//if the row failed and it was not the first row, then tell all tiles added from this
					//row that they have not been added to a group, and decrement @tilesAddedToGroups by
					//the number of tiles added in the row
					if (rowFailure  &&  currentGroup.GetHeight() != 0)
					{
						tilesAddedToGroups -= tilesAddedFromCurrentRow;
						endGroup = true;

						for (int n=x; n>startX; n--)
						{
							iter--;
							(*iter).SetAddedToGroup(false);
						}
					}
					//this was the first row for the group
					//just increment the group height and set @endX accordingly
					else if (currentGroup.GetWidth() != 0  &&  currentGroup.GetHeight() == 0)//  &&  !rowFailure)
					{
						currentGroup.SetHeight(currentGroup.GetHeight() + mTileset.GetTileWidth());

						//test the iter as it was
						iter = mTileData.begin();
						iter += (x+GetWidth()*y)-1;

						collidable = (*iter).GetCollidable();

						if (x == GetWidth()  &&  (*iter).GetLayer1() == currentLayer1Criteria)
							endX = x;
						else
							endX = x-1;
					}
					//this was another row of the group that met all the criteria
					//just increment the group height
					else if (currentGroup.GetWidth() != 0  &&  !rowFailure)
						currentGroup.SetHeight(currentGroup.GetHeight() + mTileset.GetTileWidth());						
				}

				TileGroup newGroup;
				newGroup.rect = currentGroup;
				newGroup.layer1 = currentLayer1Criteria;
				newGroup.collidable = collidable;
				tileGroups.push_back(newGroup);
			}

			return tileGroups;
		}


		/*=============================================================================
        -- Takes all of the tile groups generated in 
        =============================================================================*/
		void Map::_CreatePhysicBodies()
		{
			std::vector<TileGroup> tileGroups = _GenerateTileGroups();
			mCachedTileGroups = tileGroups;

			//create a body for every collidable tile
			physics::Environment *env = mWorld->GetPhysicsEnvironment();

			gDebug.Print(String()+"Printing out tile groups (total="+(int)tileGroups.size()+")");

			std::vector<TileGroup>::iterator iter = tileGroups.begin();
			while (iter != tileGroups.end())
			{
				if ((*iter).collidable == false)
				{
					env->CreateBox((*iter).rect.GetWidth(), (*iter).rect.GetHeight(), 1.0+physics::MARGIN_OF_ERROR,
								Vector3D<double>((*iter).rect.GetPos().x, (*iter).rect.GetPos().y, -1.0-physics::MARGIN_OF_ERROR));
				}
				else
					env->CreateBox((*iter).rect.GetWidth(), (*iter).rect.GetHeight(), 500,
								Vector3D<double>((*iter).rect.GetPos().x, (*iter).rect.GetPos().y, -1.0-physics::MARGIN_OF_ERROR));

				int width = (int)((*iter).rect.GetWidth()/mTileset.GetTileWidth());
				int height = (int)((*iter).rect.GetHeight()/mTileset.GetTileHeight());

				gDebug.Print(String()+"  Area ["+ToString(width*height)+"] Size ["+ToString(width)+"x"+ToString(height)+"] ");
				gDebug.Print(mTileset.GetTileTemplate((*iter).layer1).mName.GetStd());

				iter++;
			}
		}



		Vector3D<double> Map::GetTilePos(long id)
		{
			Vector3D<double> pos;
			Tile tile;

			//find the tile with the id
			std::vector<Tile>::iterator iter = mTileData.begin();
			while (iter !=  mTileData.end())
			{
				if ( (*iter).GetId() == id )
				{
					tile = *iter;
					break;
				}

				iter++;
			}


			//calculate x-pos
			int x = tile.GetId();
			while (x >= mWidth)
				x -= mWidth;
			pos.x = x*mTileset.GetTileWidth();


			//calculate y-pos
			int y = tile.GetId();
			y = y/mWidth;
			pos.y = y*mTileset.GetTileHeight();

			return pos;
		}

    };
};