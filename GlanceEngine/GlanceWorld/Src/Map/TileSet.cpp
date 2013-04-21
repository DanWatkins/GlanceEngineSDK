/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: TileSet.cpp
=================================================================================================*/
#include "../World.h"

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Constructor for Tileset.
        =============================================================================*/
		Tileset::Tileset()
        {
            mName       = "untitled";
            mTileWidth  = 64;
            mTileHeight = 64;
        }


        /*=============================================================================
        -- Loads a tileset file (*.ts) from disk.
        =============================================================================*/
        bool Tileset::LoadTilesetFromFile(String filename)
        {
			String filepath = filename;
			filepath.PushFront(DIR_TILESETS);
            io::Standardize(filepath);
            bool error = false;

            HMLFile file;
            file.Open(filepath);

			//header section of the file
            if (file.SeekElementOpen("<header>"))
            {
                while (!file.AtElementClose("</header>"))
                {
                    if (file.TokenIsDeclaration("#name"))
                        mName = file.SeekGetToken();
                    else if (file.TokenIsDeclaration("#image"))
                    {
                        if (!mImage.LoadImageFromFile(String(DIR_TILESETS)+file.SeekGetToken()))
                        {
                            error = true;
                            gDebug.Error(String("Could not load tileset image for the ")+mName+" tileset");
                        }
                    }
                    else if (file.TokenIsDeclaration("#tileWidth"))
                        mTileWidth = ToInt(file.SeekGetToken());
                    else if (file.TokenIsDeclaration("#tileHeight"))
                        mTileHeight = ToInt(file.SeekGetToken());

                    file.Seek();
                }
            }

			//tile property section
			if (file.SeekElementOpen("<tile_properties>"))
			{
				while (!file.AtElementClose("</tile_properties>"))
				{
					//make a new tile template
					if (file.TokenIsDeclaration("#tile"))
					{
						TileTemplate tile;

						tile.mName = file.SeekGetToken();

						if (file.SeekGetToken() == "{")
						{
							while (file.GetToken() != "}")
							{
								if (file.TokenIsDeclaration("#ref"))
									tile.mRef = (short)ToInt(file.SeekGetToken());
								else if (file.TokenIsDeclaration("#collidable"))
									tile.mCollidable = ToBool(file.SeekGetToken());

								file.Seek();
							}
						}

						mTileTemplates.push_back(tile);
					}


					file.Seek();
				}
			}

			//setup the sprite image
			mImage.SetIsSheet(true);
			mImage.SetFrameWidth(mTileWidth);
			mImage.SetFrameHeight(mTileHeight);
			mImage.SetColumns( (int)mImage.GetWidth() / mImage.GetFrameWidth() );

            return !error;
        }


		/*=============================================================================
        -- Draws an individual tile from the sheet at the specified position.
        =============================================================================*/
		void Tileset::Draw(unsigned tile, int posx, int posy)
		{
			mImage.SetFrame(tile);
			mImage.Draw(posx, posy);
		}


		/*=============================================================================
        -- 
        =============================================================================*/
		TileTemplate Tileset::GetTileTemplate(int ref)
		{
			std::vector< TileTemplate >::iterator iter = mTileTemplates.begin();
			while (iter != mTileTemplates.end())
			{
				if ( (*iter).mRef == ref )
					return *iter;

				iter++;
			}

			return TileTemplate();
		}

    };
};